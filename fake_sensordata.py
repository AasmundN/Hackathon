import csv
import random

file_name = 'sensordata.csv'

# Create a csv file that takes readings from 8 fridge temp sensors, 1 external temp sensor and 1 light sensor
# The header names should be 'fridge_temp_1', 'fridge_temp_2', 'fridge_temp_3', 'fridge_temp_4', 'fridge_temp_5', 'fridge_temp_6', 'fridge_temp_7', 'fridge_temp_8', 'external_temp_1', 'fridge_light_1'
with open(file_name, 'w') as csvfile:
    fieldnames = ['fridge_temp_1', 'fridge_temp_2', 'fridge_temp_3', 'fridge_temp_4', 'fridge_temp_5',
                  'fridge_temp_6', 'fridge_temp_7', 'fridge_temp_8', 'external_temp_1', 'fridge_light_1']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()

    # Write 100 rows of fake data
    for i in range(100):
        writer.writerow({
            'fridge_temp_1': random.randint(0, 10),
            'fridge_temp_2': random.randint(0, 10),
            'fridge_temp_3': random.randint(0, 10),
            'fridge_temp_4': random.randint(0, 10),
            'fridge_temp_5': random.randint(0, 10),
            'fridge_temp_6': random.randint(0, 10),
            'fridge_temp_7': random.randint(0, 10),
            'fridge_temp_8': random.randint(0, 10),
            'external_temp_1': random.randint(0, 30),
            'fridge_light_1': random.randint(0, 1023),
        })
