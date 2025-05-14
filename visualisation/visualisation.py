import json
import pandas as pd
import matplotlib as plt

from Extractor import Extractor, SimData

JSON_FILE = "json/main_controller.json"

# Extract the information
def extract_info(data) -> SimData:
    extractor = Extractor(data)
    return extractor.extracted_data

# Produce chart
def produce_chart():
    pass

# Plot metrics
def plot_metrics():
    pass

if __name__ == "__main__":
    print("\nVisualising results...")

    # Open the file
    with open(JSON_FILE) as json_file:
        data = json.load(json_file)
    
    # Extract data
    structured_data = extract_info(data)
    print("Parsed data")