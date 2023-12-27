import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

# Step 1: Read the CSV file without headers since we didn't label the columns
df = pd.read_csv('orderbook_snapshots.csv', header=None)

# - The first column (index 0) is 'timestamp'
# - The third column (index 2) is 'price level'
# - The fifth column (index 4) contains types of order (bid/ask) 

# Step 2: Filter the data for best asks and best bids
best_asks = df[df[4] == '0_ask']
best_bids = df[df[4] == '0_bid']

# Step 3: Extract price levels
best_asks_prices = best_asks[2]
best_bids_prices = best_bids[2]

# Step 4: Visualize the data
# Plotting the price levels over time for best asks and bids
plt.figure(figsize=(12, 6))
sns.lineplot(data=best_asks, x=0, y=2, label='Best Asks')
sns.lineplot(data=best_bids, x=0, y=2, label='Best Bids')
plt.title('Orderbook Best Asks and Bids Over Time (possibly 10-15 minutes)')
plt.xlabel('Time')
plt.ylabel('Price')
plt.xticks(rotation=45)
plt.legend()
plt.show()
