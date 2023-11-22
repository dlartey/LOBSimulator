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

load_dotenv(dotenv_path="example.env")

API_KEY = "PqxVt8Ww7xTAh9Lg"
SECRET_KEY = "j6GXlsFTCqHCo9QJ2JM2ezqVrll10Az8"

URI = "wss://advanced-trade-ws.coinbase.com"

channel = "level2"

product_id = "ETH-USD"
agg_level = "0.1"

async def main_loop():
    last_written_timestamp = None
    start_time = time.time()  # Record the start time

    try:
        async with websockets.connect(URI, ping_interval=None, max_size=None) as websocket:
            auth_message = await create_auth_message(channel, product_id, str(int(time.time())))
            await websocket.send(auth_message)

            processor = None
            while True:
                response = await websocket.recv()
                parsed = json.loads(response)

                if parsed["channel"] == "l2_data" and parsed["events"][0]["type"] == "snapshot":
                    processor = OrderBookProcessor(response)
                elif processor is not None:
                    processor.apply_update(response)

                if processor is not None:
                    timestamp, snapshot = processor.get_snapshot_with_timestamp()
                    current_snapshot_second = timestamp[:19]

                    if current_snapshot_second != last_written_timestamp:
                        with open('orderbook_snapshots.csv', 'a', newline='') as file:
                            writer = csv.writer(file)
                            for index, row in snapshot.iterrows():
                                writer.writerow([timestamp] + list(row))
                        last_written_timestamp = current_snapshot_second

                # Check if a minute has passed since the start time
                if time.time() - start_time >= 60:
                    break  # Exit the loop after a minute

    except websockets.ConnectionClosed:
        pass


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
    asyncio.run(main_loop())
