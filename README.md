# arcvynt, a portfolio tracker in your terminal.
Arcvynt is a lightweight portfolio tracker that tracks your portfolio worth in real-time. <br>
Preview: <br>
![image](https://github.com/user-attachments/assets/4649fea3-c7a2-44c0-aa51-58159f127c50)

# Set-up:
To set-up the program, simply replace the contents of 'portf.txt' with your own data. The ticker names can be found on Yahoo-finance. The following format is expected: <br>
ticker,amount,currency <br>
ticker,amount,currency <br>
... <br>
ticker,amount,currency <br>
<br>
Where ticker is as given by Yahoo-Finance, amount is an integer, and currency is either EUR or USD, depending on the currency shown on the stock name's Yahoo-Finance page. <br>
Example: <br>
```
AMD,3,USD
VUSA.AS,31,EUR
IWDA.AS,5,EUR
EUREUR=X,30,EUR
BTC-USD,1,USD
```

# Compiling
To compile, simply run the 'make' command. To output valuta in euro's, run 'make use_eur'. An executable should now be in your folder.

# Customizing
To change or remove the ASCII image, simply edit 'img.txt'. To change the image color, change the IMGCOL variable. To change column width, edit the COLWIDTH array variable.
