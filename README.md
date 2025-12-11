# portfolio-risk-c

A portfolio analysis tool written in C, designed to connect core computer science skills (file I/O, memory management, arrays, numerical methods) with asset managemnet concepts
such as expected return, volatility, diversification and covariance.
This project reads historical price data from a CSV file, computes daily returns, annualises expected returns/risk and allows users to eneter portfolio weights to calculate the overall portfolio's expected performance.


As a first year computer science student exploring the connection programming finance, I built this project to better understand how portfolio construction works at a mathematical level.
Asset managers think in terms of:

Return – “How much do we expect to make?”
Risk (Volatility) – “How much does it fluctuate?”
Covariance – “How do assets move together?”
Weighting – “How much of each asset should we hold?”

This tool demonstrates how these ideas can be implemented using programming call mom without relying on external finance libraries.

Project Features
- Reads real (or sample) market price data from a CSV file in the format:

Date,AAPL,MSFT,VOO

2024-01-02,100,150,200

...

- Computes financial statistics from scratch
  - Daily returns
  - Annualised expected returns
  - Covariance matrix of returns
  - Portfolio expected return
  - Portfolio volatility (risk)
 
- Allows user-defined portfolio weights
  The program accepts weights that sum up to 1.0 (or normalises them automatically) and outputs:
   - Final portfolio expected return
   - Final portfolio volatility
   - Weight breakdown
 
- Written with finance recruiters in mind
  The entire project runs on standard C, making it portable and easy to compile.

How to Build & Run the Program
1. Clone or download the repository:


2. Ensure the prices.csv file is inside the data/ folder.


3. Compile using the Makefile:

   make

   This generates and executable called portfolio.

4. Run the program:

   ./portfolio

5. Enter portfolio weights when prompted.

The program then outputs:
- Expected annual return
- Annual volatility
- Normalised weights

Sample Output:

=== Portfolio Risk & Return Calculator (C) ===


Loading price data from 'data/prices.csv'...


Loaded 10 days of prices for 3 assets.


Assets and expected annual returns (approx):

  AAPL: 217.05%
  
  MSFT: 128.97%
  
  VOO: 177.22%


Enter portfolio weights for each asset.

They should sum to 1.0 (100% total).


Weight for AAPL (as decimal, e.g. 0.3): 0.4

Weight for MSFT (as decimal, e.g. 0.3): 0.4

Weight for VOO (as decimal, e.g. 0.3): 0.2


Sum of weights entered: 1.0000


=== Portfolio Result ===

Weights:

  AAPL: 40.00%
  
  MSFT: 40.00%
  
  VOO: 20.00%


Expected annual return: 173.85%

Annual volatility (risk): 6.48%


Future Extensions:

 - Add correlation matrix output
 - Implement automatic equal-weight portfolio
 - Add efficient frontier generation
 - Allow importing larger real datasets
 - Introduce Sharpe ratio calculation
 - Add command-line arguments to change file paths or asset selection

Author
Labib Ramdan
First-Year Computer Science Student
Interested in Quantitative Finance, Asset Management and Data Analysis


  
