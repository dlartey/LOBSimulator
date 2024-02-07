from PyQt6.QtGui import QColor
from PyQt6.QtWidgets import QApplication, QMainWindow, QTableWidget, QTableWidgetItem, QVBoxLayout, QHBoxLayout, QWidget
from PyQt6.QtCharts import QChart, QCandlestickSeries, QCandlestickSet, QChartView, QValueAxis, QDateTimeAxis
from PyQt6.QtCore import QTimer, Qt, QDateTime
import pandas as pd
import os


class TraderWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Trader Window")
        self.setGeometry(100, 100, 1200, 800)

        # Main layout
        mainLayout = QVBoxLayout()
        tablesLayout = QHBoxLayout()  # Layout for bid and ask tables

        centralWidget = QWidget(self)
        self.setCentralWidget(centralWidget)
        centralWidget.setLayout(mainLayout)

        # Bid Table
        self.bidTable = QTableWidget()
        self.bidTable.setHorizontalHeaderLabels(['Timestamp', 'ID', 'Price', 'Quantity', 'Type'])
        tablesLayout.addWidget(self.bidTable)

        # Ask Table
        self.askTable = QTableWidget()
        self.askTable.setHorizontalHeaderLabels(['Timestamp', 'ID', 'Price', 'Quantity', 'Type'])
        tablesLayout.addWidget(self.askTable)

        mainLayout.addLayout(tablesLayout)  # Add bid/ask tables layout to the main layout

        # Candlestick Chart
        self.chartView = QChartView()
        mainLayout.addWidget(self.chartView)

        # Timer setup for periodic updates
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.updateData)
        self.timer.start(100)  # Update every 0.5 seconds

    def updateOrderbookTables(self, csv_path):
        df = pd.read_csv(csv_path, header=None, names=['Timestamp', 'ID', 'Price', 'Quantity', 'Type'])

        # Filter rows for bids and asks
        bids = df[df['Type'].str.contains('bid')]
        asks = df[df['Type'].str.contains('ask')]

        # Reset the index to avoid empty rows due to filtering
        bids.reset_index(drop=True, inplace=True)
        asks.reset_index(drop=True, inplace=True)

        # Update bid table
        self.updateTable(self.bidTable, bids)

        # Update ask table
        self.updateTable(self.askTable, asks)

    def updateData(self):
        # Path to your CSV file
        csv_path = 'orderbook_snapshots.csv'  # Ensure this path is correct
        self.updateOrderbookTables(csv_path)
        self.createCandlestickChart()  # Call this method to create and display the chart

    def updateOrderbookTables(self, csv_path):
        # Read the CSV without assigning column names
        df = pd.read_csv(csv_path, header=None)

        # Assuming the 'Type' is in the 5th column (index 4 since pandas uses 0-based indexing)
        bids = df[df.iloc[:, 4].str.contains('bid', na=False)]
        asks = df[df.iloc[:, 4].str.contains('ask', na=False)]

        # Reset the index to avoid empty rows due to filtering
        bids.reset_index(drop=True, inplace=True)
        asks.reset_index(drop=True, inplace=True)

        # Update bid table
        self.updateTable(self.bidTable, bids)

        # Update ask table
        self.updateTable(self.askTable, asks)

    @staticmethod
    def updateTable(table: QTableWidget, data: pd.DataFrame):
        table.clearContents()
        table.setRowCount(data.shape[0])
        table.setColumnCount(data.shape[1])
        for rowIndex in range(data.shape[0]):
            for colIndex in range(data.shape[1]):
                item = data.iloc[rowIndex, colIndex]
                if pd.notnull(item):
                    table.setItem(rowIndex, colIndex, QTableWidgetItem(str(item)))

    def createCandlestickChart(self):
        try:
            # Read the data from the CSV file, assuming that it has no header
            df = pd.read_csv('orderbook_snapshots.csv', header=None)
            df.columns = ['Timestamp', 'ID', 'Price', 'Quantity', 'Type']  # Assign column names
            df['Timestamp'] = pd.to_datetime(df['Timestamp'])

            # Sort the DataFrame by timestamp
            df.sort_values('Timestamp', inplace=True)

            # Group the data into 5-second intervals and calculate OHLC
            ohlc = df.resample('5s', on='Timestamp').agg({
                'Price': ['first', 'max', 'min', 'last']
            }).dropna()

            # Flatten the column hierarchy
            ohlc.columns = ['Open', 'High', 'Low', 'Close']

            # Create a new candlestick series
            series = QCandlestickSeries()
            series.setName('OHLC')
            series.setIncreasingColor(QColor('green'))  # Use QColor for color specification
            series.setDecreasingColor(QColor('red'))  # Use QColor for color specification

            # Add data to the series
            for timestamp, row in ohlc.iterrows():
                # Get the POSIX timestamp and convert to QDateTime
                posix_timestamp = int(timestamp.timestamp())
                qtimestamp = QDateTime.fromSecsSinceEpoch(posix_timestamp)
                candlestick = QCandlestickSet(row['Open'], row['High'], row['Low'], row['Close'],
                                              posix_timestamp * 1000)  # PyQt6 expects milliseconds
                series.append(candlestick)

            # Create the chart and add the series to it
            chart = QChart()
            chart.addSeries(series)
            chart.setTitle("5 Second OHLC Candlestick Chart")
            chart.createDefaultAxes()

            # Configure the axes
            axisX = chart.axes(Qt.Orientation.Horizontal)[0]
            axisY = chart.axes(Qt.Orientation.Vertical)[0]

            if isinstance(axisX, QDateTimeAxis):
                axisX.setFormat("HH:mm:ss")
                axisX.setTitleText("Time")

            if isinstance(axisY, QValueAxis):
                axisY.setLabelFormat("%.2f")
                axisY.setTitleText("Price")

            # Set the chart on the chart view
            self.chartView.setChart(chart)

        except Exception as e:
            print(f"An error occurred: {e}")


if __name__ == "__main__":
    app = QApplication([])
    window = TraderWindow()
    window.show()
    app.exec()
