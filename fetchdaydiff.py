import sys
import yfinance as yf

def get_stock_price(ticker):
    try:
        stock = yf.Ticker(ticker)
        todays_data = stock.history(period='2d')
        todays_data = todays_data['Close'].iloc[0] - todays_data['Close'].iloc[-1]
        return todays_data 
    except Exception as e:
        return None

if __name__ == "__main__":
    ticker = sys.argv[1]
    current_price = get_stock_price(ticker)
    if current_price is not None:
        print(current_price)
