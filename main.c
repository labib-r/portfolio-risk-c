#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_DAYS 365
#define MAX_ASSETS 10
#define LINE_LEN 256

int load_prices(const char *filename, double prices[MAX_DAYS][MAX_ASSETS], int *num_days, int *num_assets, char asset_names[MAX_ASSETS][16]);
int compute_returns(double prices[MAX_DAYS][MAX_ASSETS], int num_days, int num_assets, double returns[MAX_DAYS - 1][MAX_ASSETS]);
void compute_mean_returns(double returns[MAX_DAYS - 1][MAX_ASSETS], int num_returns, int num_assets, double mean_returns[MAX_ASSETS]);
void compute_covariance(double returns[MAX_DAYS - 1][MAX_ASSETS], int num_returns, int num_assets, double cov[MAX_ASSETS][MAX_ASSETS]);
void print_assets(int num_assets, char asset_names[MAX_ASSETS][16], double mean_returns[MAX_ASSETS]);
void compute_portfolio(double mean_returns[MAX_ASSETS], double cov[MAX_ASSETS][MAX_ASSETS], double *weights, int num_assets, double *port_return, double *port_vol);

int main(void)
{
    double prices[MAX_DAYS][MAX_ASSETS];
    double returns[MAX_DAYS - 1][MAX_ASSETS];
    double mean_returns[MAX_ASSETS];
    double cov[MAX_ASSETS][MAX_ASSETS];
    char asset_names[MAX_ASSETS][16];

    int num_days = 0;
    int num_assets = 0;

    const char *filename = "data/prices.csv";

    printf("=== Portfolio Risk & Return Calculator (C) ===\n");
    printf("Loading price data from '%s'...\n\n", filename);

    if (num_days < 3) {
        fprintf(stderr, "Not enough data to compute returns and covariance.\n");
        free(weights);
        return 1;
    }

    if (!load_prices(filename, prices, &num_days, &num_assets, asset_names)) {
        fprintf(stderr, "Error: failed to load price data.\n");
        return 1;
    }

    int num_returns = compute_returns(prices, num_days, num_assets, returns);

    compute_mean_returns(returns, num_returns, num_assets, mean_returns);
    compute_covariance(returns, num_returns, num_assets, cov);

    printf("Loaded %d days of prices for %d assets.\n\n", num_days, num_assets);
    print_assets(num_assets, asset_names, mean_returns);

    double *weights = malloc(num_assets * sizeof(double));
    if (weights == NULL) {
        fprintf(stderr, "Memory allocation error.\n");
        return 1;
    }

    printf("\nEnter portfolio weights for each asset.\n");
    printf("They should sum to 1.0 (100%% total).\n\n");

    double sum = 0.0;
    for (int j = 0; j < num_assets; j++) {
        printf("Weight for %s (as decimal, e.g. 0.3): ", asset_names[j]);
        if (scanf("%lf", &weights[j]) != 1) {
            fprintf(stderr, "Invalid input.\n");
            free(weights);
            return 1;
        }
        sum += weights[j];
    }

    printf("\nSum of weights entered: %.4f\n", sum);
    if (fabs(sum - 1.0) > 0.0001) {
        printf("Warning: weights do not sum exactly to 1.0. "
               "They will be normalised.\n");
        for (int j = 0; j < num_assets; j++) {
            weights[j] /= sum;
        }
    }

    double port_return = 0.0;
    double port_vol = 0.0;
    compute_portfolio(mean_returns, cov, weights, num_assets, &port_return, &port_vol);

    printf("\n=== Portfolio Result ===\n");
    printf("Weights:\n");
    for (int j = 0; j < num_assets; j++) {
        printf("  %s: %.2f%%\n", asset_names[j], weights[j] * 100.0);
    }
    printf("\nExpected annual return: %.2f%%\n", port_return * 100.0);
    printf("Annual volatility (risk): %.2f%%\n", port_vol * 100.0);

    free(weights);
    return 0;
}

int load_prices(const char *filename, double prices[MAX_DAYS][MAX_ASSETS],
                int *num_days, int *num_assets,
                char asset_names[MAX_ASSETS][16])
{
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        perror("fopen");
        return 0;
    }

    char line[LINE_LEN];

    if (fgets(line, sizeof(line), f) == NULL) {
        fclose(f);
        return 0;
    }

    int assets = 0;
    char *token = strtok(line, ",\n");
    int col = 0;
    while (token != NULL && assets < MAX_ASSETS + 1) {
        if (col > 0) {
            strncpy(asset_names[assets], token, 15);
            asset_names[assets][15] = '\0';
            assets++;
        }
        token = strtok(NULL, ",\n");
        col++;
    }

    if (assets == 0) {
        fclose(f);
        return 0;
    }

    int days = 0;
    while (fgets(line, sizeof(line), f) != NULL && days < MAX_DAYS) {
        col = 0;
        token = strtok(line, ",\n");
        while (token != NULL && col < assets + 1) {
            if (col > 0) {
                int asset_index = col - 1;
                prices[days][asset_index] = atof(token);
            }
            token = strtok(NULL, ",\n");
            col++;
        }
        if (col >= assets + 1) {
            days++;
        }
    }

     fclose(f);

    *num_days = days;
    *num_assets = assets;
    return 1;
}

int compute_returns(double prices[MAX_DAYS][MAX_ASSETS],
                    int num_days, int num_assets,
                    double returns[MAX_DAYS - 1][MAX_ASSETS])
{
    int num_returns = num_days - 1;
    for (int t = 1; t < num_days; t++) {
        for (int j = 0; j < num_assets; j++) {
            double p_t = prices[t][j];
            double p_prev = prices[t - 1][j];
            returns[t - 1][j] = (p_t / p_prev) - 1.0;
        }
    }
    return num_returns;
}

void compute_mean_returns(double returns[MAX_DAYS - 1][MAX_ASSETS],
                          int num_returns, int num_assets,
                          double mean_returns[MAX_ASSETS])
{
    for (int j = 0; j < num_assets; j++) {
        double sum = 0.0;
        for (int t = 0; t < num_returns; t++) {
            sum += returns[t][j];
        }
        double mean_daily = sum / num_returns;
        mean_returns[j] = mean_daily * 252.0;
    }
}

void compute_covariance(double returns[MAX_DAYS - 1][MAX_ASSETS],
                        int num_returns, int num_assets,
                        double cov[MAX_ASSETS][MAX_ASSETS])
{
    double mean_daily[MAX_ASSETS];

    for (int j = 0; j < num_assets; j++) {
        double sum = 0.0;
        for (int t = 0; t < num_returns; t++) {
            sum += returns[t][j];
        }
        mean_daily[j] = sum / num_returns;
    }

    for (int i = 0; i < num_assets; i++) {
        for (int j = 0; j < num_assets; j++) {
            double sum = 0.0;
            for (int t = 0; t < num_returns; t++) {
                double di = returns[t][i] - mean_daily[i];
                double dj = returns[t][j] - mean_daily[j];
                sum += di * dj;
            }
            double cov_daily = sum / (num_returns - 1);
            cov[i][j] = cov_daily * 252.0; 
        }
    }
}

void print_assets(int num_assets, char asset_names[MAX_ASSETS][16],
                  double mean_returns[MAX_ASSETS])
{
    printf("Assets and expected annual returns (approx):\n");
    for (int j = 0; j < num_assets; j++) {
        printf("  %s: %.2f%%\n", asset_names[j], mean_returns[j] * 100.0);
    }
}

void compute_portfolio(double mean_returns[MAX_ASSETS],
                       double cov[MAX_ASSETS][MAX_ASSETS],
                       double *weights, int num_assets,
                       double *port_return, double *port_vol)
{
    double r = 0.0;
    for (int j = 0; j < num_assets; j++) {
        r += weights[j] * mean_returns[j];
    }

    double var = 0.0;
    for (int i = 0; i < num_assets; i++) {
        for (int j = 0; j < num_assets; j++) {
            var += weights[i] * cov[i][j] * weights[j];
        }
    }

    *port_return = r;
    *port_vol = sqrt(var);
}
