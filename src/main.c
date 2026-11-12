
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pigpio.h>

#define LED_PIN 17
#define LOG_FILE "data/temperature_log.csv"
#define STATUS_FILE "status.txt"

double minimum = 68.0;
double maximum = 76.0;
double total = 0.0;
int count = 0;

void recordTemperature() {
    double temperature;

    printf("Enter temperature (F): ");
    if (scanf("%lf", &temperature) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    int warning = temperature < minimum || temperature > maximum;
    const char *status = warning ? "WARNING" : "NORMAL";

    gpioWrite(LED_PIN, warning);

    FILE *log = fopen(LOG_FILE, "a");
    if (log != NULL) {
        fprintf(log, "%.2f,%s\n", temperature, status);
        fclose(log);
    }

    FILE *statusFile = fopen(STATUS_FILE, "w");
    if (statusFile != NULL) {
        fprintf(statusFile, "%s\n", status);
        fclose(statusFile);
    }

    total += temperature;
    count++;

    printf("Temperature: %.2f F\n", temperature);
    printf("Status: %s\n", status);
    printf("LED: %s\n", warning ? "ON" : "OFF");
}

int main() {
    if (gpioInitialise() < 0) {
        printf("GPIO initialization failed.\n");
        return 1;
    }

    gpioSetMode(LED_PIN, PI_OUTPUT);
    gpioWrite(LED_PIN, 0);

    int choice;

    do {
        printf("\n=== CLASSROOM COMFORT MONITOR ===\n");
        printf("1. Enter temperature\n");
        printf("2. Show average\n");
        printf("3. Change acceptable range\n");
        printf("4. Exit\n");
        printf("Choose: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = 0;
        }

        switch (choice) {
            case 1:
                recordTemperature();
                break;

            case 2:
                if (count > 0)
                    printf("Average: %.2f F\n", total / count);
                else
                    printf("No readings yet.\n");
                break;

            case 3:
                printf("New minimum: ");
                scanf("%lf", &minimum);
                printf("New maximum: ");
                scanf("%lf", &maximum);
                break;

            case 4:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 4);

    gpioWrite(LED_PIN, 0);
    gpioTerminate();

    return 0;
}

