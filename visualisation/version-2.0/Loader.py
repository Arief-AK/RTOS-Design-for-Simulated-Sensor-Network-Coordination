import json
import pandas as pd

class Loader:
    def __init__(self):
        self.raw_data = {}

    def read_json_data(self, file_path: str, key: str) -> bool:
        success = False
        try:
            df = pd.read_json(file_path)
            self.raw_data.update({key:df})
        except Exception as e:
            print(f"Error reading JSON data from {file_path}: {e}")
        else:
            success = True
            print(f"Data read successfully from {file_path}")
        return success

    def load_json_data(self, file_path: str, key: str) -> bool:
        success = False
        with open(file_path) as json_file:
            try:
                summary_nested = json.load(json_file)
                data = pd.json_normalize(summary_nested[0])
                self.raw_data.update({key: data})
            except json.JSONDecodeError as e:
                print(f"Error decoding JSON from {file_path}: {e}")
            except Exception as e:
                print(f"Unexpected error while loading data from {file_path}: {e}")
            else:
                success = True
                print(f"Data loaded successfully from {file_path}")
        return success

    def get_data(self) -> pd.DataFrame:
        return self.raw_data
