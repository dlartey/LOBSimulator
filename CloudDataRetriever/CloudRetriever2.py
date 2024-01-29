# Copyright 2023-present Coinbase Global, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
import asyncio, websockets, sqlite3, json, hmac, hashlib, base64, os, time, sys, csv
from dotenv import load_dotenv
from orderbook import OrderBookProcessor
from datetime import datetime
import warnings
import pandas as pd
import logging

load_dotenv(dotenv_path="example.env")

API_KEY = "PqxVt8Ww7xTAh9Lg"
SECRET_KEY = "j6GXlsFTCqHCo9QJ2JM2ezqVrll10Az8"

URI = "wss://advanced-trade-ws.coinbase.com"

channel = "level2"

product_id = "ETH-USD"
agg_level = "0.1"


start_time = time.time()
processor = None

# Configure logging
logging.basicConfig(
    filename='LogFile',  # Specify the name of the log file
    filemode='a',  # Use 'a' to append to the log file
    format='%(asctime)s [%(levelname)s]: %(message)s',  # Customize the log message format
    datefmt='%Y-%m-%d %H:%M:%S',  # Customize the date/time format
    level=logging.DEBUG  # Set the logging level (DEBUG, INFO, WARNING, ERROR, CRITICAL)
)


async def main_loop(seconds, additional, bestNum):
    global start_time
    start_time = time.time()
    while True:
        try:
            if bestNum == 0:
                await _main_loop(seconds, additional)
            else:
                await _main_loop2(seconds, additional, bestNum)
        except websockets.ConnectionClosed as e:
            print(f"WebSocket connection closed unexpectedly: {e}")
            await asyncio.sleep(5)
            
async def _main_loop(seconds, additional):
    last_written_timestamp = None
    global start_time
    global processor
    Max = seconds+additional
    s_time = pd.to_datetime(start_time, unit='s')
    
    print('Start: ',start_time)
    logging.info(f"Start: {start_time}")
    print('MAX: ', Max)
    logging.info(f"MAX: {Max}")
    
    try:
        logging.info("Connecting to websocket")
        async with websockets.connect(URI, ping_interval=None, max_size=None) as websocket:
            auth_message = await create_auth_message(channel, product_id, str(int(time.time())))
            await websocket.send(auth_message)
            #start_time = time.time()
            
            while True:
                logging.info("Waiting for Data")
                response = await websocket.recv()
                logging.info(f"Parsing Data, size of response: {sys.getsizeof(response)}")
                parsed = json.loads(response)
                logging.info(f"Parsing Data, size of parsed: {sys.getsizeof(parsed)}")
                logging.info("Data Parsed")

                if parsed["channel"] == "l2_data" and parsed["events"][0]["type"] == "snapshot":
                    logging.info("Creating Processor OrderBook")
                    processor = OrderBookProcessor(response)
                    logging.info(f"size of processor: {sys.getsizeof(processor)}")
                elif processor is not None:
                    logging.info("Updating Processor Order Book")
                    processor.apply_update(response)
                    logging.info(f"size of processor: {sys.getsizeof(processor)}")

                if processor is not None:
                    timestamp, snapshot = processor.get_snapshot_with_timestamp()
                    current_snapshot_second = timestamp[:19]

                    if current_snapshot_second != last_written_timestamp:
                        with open('orderbook_snapshots.csv', 'a', newline='') as file:
                            writer = csv.writer(file)
                            for index, row in snapshot.iterrows():
                                timestamp_dt = datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S")
                                formatted_timestamp = timestamp_dt.strftime("%Y-%m-%d %H:%M:%S")
                                
                                formated_price = float(row.iloc[1])
                                row.iloc[1] = round(formated_price, 2)
                                
                                formated_quantity = float(row.iloc[2])
                                row.iloc[2] = round(formated_quantity, 6)
                                writer.writerow([formatted_timestamp] + list(row))
                                
                            file.flush()
                            os.fsync(file.fileno())
                            file.close()
                        last_written_timestamp = current_snapshot_second
                        
                        

                # Check if the timelap has passed since the start time
                if time.time() - start_time >= (Max):
                    logging.info("Calling delete_rows method")
                    delete_rows_in_time_range('orderbook_snapshots.csv', s_time, additional)
                    start_time = start_time + additional
                    s_time = pd.to_datetime(start_time, unit='s')
                    logging.info("Deletion Complete")
                    
    except websockets.ConnectionClosed as e:
        print(f"WebSocket connection closed unexpectedly: {e}") 
        logging.error(f"WebSocket connection closed unexpectedly: {e}") 
    
    except Exception as e:
        print(f"An error occurred: {e}")
        logging.error(f"An error occurred: {e}")

async def _main_loop2(seconds, additional, bestNum):
    last_written_timestamp = None
    global start_time
    global processor
    Max = seconds+additional
    s_time = pd.to_datetime(start_time, unit='s')
    
    print('Start: ',start_time)
    logging.info(f"Start: {start_time}")
    print('MAX: ', Max)
    logging.info(f"MAX: {Max}")
    
    try:
        async with websockets.connect(URI, ping_interval=None, max_size=None) as websocket:
            auth_message = await create_auth_message(channel, product_id, str(int(time.time())))
            await websocket.send(auth_message)
            #start_time = time.time()
            
            while True:
                logging.info("Waiting for Data")
                response = await websocket.recv()
                logging.info(f"Parsing Data, size of response: {sys.getsizeof(response)}")
                parsed = json.loads(response)
                logging.info(f"Parsing Data, size of parsed: {sys.getsizeof(parsed)}")
                logging.info("Data Parsed")

                if parsed["channel"] == "l2_data" and parsed["events"][0]["type"] == "snapshot":
                    logging.info("Creating Processor OrderBook")
                    processor = OrderBookProcessor(response)
                    logging.info(f"size of processor: {sys.getsizeof(processor)}")
                elif processor is not None:
                    logging.info("Updating Processor Order Book")
                    processor.apply_update(response)
                    logging.info(f"size of processor: {sys.getsizeof(processor)}")

                if processor is not None:
                    timestamp, snapshot = processor.get_snapshot_with_timestamp()
                    current_snapshot_second = timestamp[:19]

                    if current_snapshot_second != last_written_timestamp:
                        with open('orderbook_snapshots.csv', 'a', newline='') as file:
                            writer = csv.writer(file)
                            filtered_rows = snapshot[
                            (
                                (snapshot.iloc[:, -1].str.endswith('_ask') &  # Check if it ends with '_ask'
                                snapshot.iloc[:, -1].str.extract('(\d+)', expand=False).astype(float).between(0, bestNum-1))
                            ) |
                            (
                                (snapshot.iloc[:, -1].str.endswith('_bid') &  # Check if it ends with '_bid'
                                snapshot.iloc[:, -1].str.extract('(\d+)', expand=False).astype(float).between(0, bestNum-1))
                            )
                            ]
                            for index, row in filtered_rows.iterrows():   
                                timestamp_dt = datetime.strptime(timestamp, "%Y-%m-%d %H:%M:%S")
                                formatted_timestamp = timestamp_dt.strftime("%Y-%m-%d %H:%M:%S")
                                
                                formated_price = float(row.iloc[1])
                                row.iloc[1] = round(formated_price, 2)
                                
                                formated_quantity = float(row.iloc[2])
                                row.iloc[2] = round(formated_quantity, 6)
                                writer.writerow([formatted_timestamp] + list(row))        
                                 
                            file.flush()
                            os.fsync(file.fileno())
                            file.close()
                        last_written_timestamp = current_snapshot_second
                        
                        

                # Check if the timelap has passed since the start time
                if time.time() - start_time >= (Max):
                    
                    logging.info("Calling delete_rows method")
                    delete_rows_in_time_range('orderbook_snapshots.csv', s_time, additional)
                    start_time = start_time + additional
                    s_time = pd.to_datetime(start_time, unit='s')
                    
                    logging.info("Deletion Complete")
                    
    except websockets.ConnectionClosed as e:
        print(f"WebSocket connection closed unexpectedly: {e}") 
        logging.error(f"WebSocket connection closed unexpectedly: {e}") 
    
    except Exception as e:
        print(f"An error occurred: {e}")
        logging.error(f"An error occurred: {e}")
        
    
def delete_rows_in_time_range(csv_file, start_time, additional):
    # Read the CSV file into a pandas DataFrame
    df = pd.read_csv(csv_file, header=None)  # Specify header=None to indicate there's no header
    start_time = pd.to_datetime(start_time)
    end_time = start_time + pd.to_timedelta(additional, unit='s')
    
    print("START: ", start_time)
    print("END: ", end_time)

    # Convert the timestamp column to datetime for comparison
    df[0] = pd.to_datetime(df[0])

    # Drop rows based on the specified time range
    df = df.drop(df[(df[0] >= start_time) & (df[0] <= end_time)].index)

    # Write the modified DataFrame back to the CSV file
    df.to_csv(csv_file, index=False, header=False)  # Specify header=False to write without header
    


async def create_auth_message(channel, product_id, timestamp):
    signature = sign(channel, API_KEY, product_id, timestamp)
    auth_message = json.dumps({
        "type": "subscribe",
        "channel": channel,
        "api_key": API_KEY,
        "timestamp": timestamp,
        "signature": signature.hex(),
        "product_ids": [product_id],
    })
    return auth_message


def sign(channel, key, product_ids, timestamp):
    message = timestamp + channel + product_ids
    signature = hmac.new(SECRET_KEY.encode("utf-8"), message.encode("utf-8"), digestmod=hashlib.sha256).digest()
    return signature


if __name__ == "__main__":
    warnings.filterwarnings("ignore", category=FutureWarning)
    asyncio.run(main_loop(14400, 600, 1))
