from flask import Flask, send_file
import pandas as pd
import time

app = Flask(__name__)

@app.route('/')
def get_latest_excel():
    return send_file('orderbook_snapshots.csv', as_attachment=True)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
