import sys
import yfinance as yf

def get_stock_price(ticker):
    try:
        stock = yf.Ticker(ticker)
        todays_data = stock.history(period='1d')
        current_price = todays_data['Close'].iloc[0]
        return current_price
    except Exception as e:
        return None

if __name__ == "__main__":
    ticker = sys.argv[1]
    current_price = get_stock_price(ticker)
    if current_price is not None:
        print(current_price)
