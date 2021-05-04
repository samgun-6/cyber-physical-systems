# Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt


def comparisonSteering(mergedFile):

    # Adding a new column with Boolean if the Steering Agnle is within range
    mergedFile['InRange'] = (mergedFile['Lower'] < mergedFile['Asteeringangle']) & (mergedFile['Upper'] > mergedFile['Asteeringangle'])

    # printing the result
    print("This is the equality")
    print(mergedFile.InRange.value_counts())


def plotLineGraphComp(ourFile, originalFile):

    # adding the data to plot
    plt.plot(originalFile['groundSteering'], label = "Expected")
    plt.plot(ourFile['groundSteering'], label = "Actual")

    # plotting 
    plt.ylabel('Steering Angel')
    plt.xlabel('N timestamps')
    plt.title('Comparison of steering angel')
    plt.legend()

    plt.show()


def sliceAndCalculateTimestamp(df):

    # converting sampleTimeStap columns to string to then concatenate them with a comma
    df['sampleTimeStamp.seconds'] = df['sampleTimeStamp.seconds'].astype(str)
    df['sampleTimeStamp.microseconds'] = df['sampleTimeStamp.microseconds'].astype(str)
    df['Timestamp'] = df['sampleTimeStamp.seconds'] + df['sampleTimeStamp.microseconds']

    # Adding new columns with threshold values 
    df['Lower'] = df['groundSteering'] / 2.0
    df['Upper'] = df['groundSteering'] * 1.5

    # Adding threshold values for columns with value 0
    df.loc[df['groundSteering'] == 0, 'Upper'] = 0.05
    df.loc[df['groundSteering'] == 0, 'Lower'] = -0.05

    # deleting redundant columns
    del df['sent.seconds']
    del df['sent.microseconds']
    del df['received.seconds']
    del df['received.microseconds']
    del df['Unnamed: 7']
    del df['sampleTimeStamp.seconds']
    del df['sampleTimeStamp.microseconds']

    # casting the Timestamp to a float
    df['Timestamp'] = df['Timestamp'].astype('int64')

    # returing the dataframe
    return df

def sliceAndCalculateTimestampImportedFile(df):

    # converting sampleTimeStap columns to string to then concatenate them with a comma

    df['groundSteering'] = df['calculatedSteeringAngle']
    df['Timestamp'] = df['sampleTimeStamp.microseconds']

    # deleting redundant columns

    del df['Unnamed: 2']
    del df['calculatedSteeringAngle']
    del df['sampleTimeStamp.microseconds']

    # casting the Timestamp to a float
    df['Timestamp'] = df['Timestamp'].astype('float64')

    # returing the dataframe
    return df

# importing csvs
csvFile = pd.read_csv("calculated-steering-angle.csv", sep = ";")
csvFileOriginal = pd.read_csv("1456.csv", sep = ";")

# Slicing and reformatting into dataframes
csvFile = sliceAndCalculateTimestampImportedFile(csvFile)
csvFileOriginal = sliceAndCalculateTimestamp(csvFileOriginal)

# Merging the dataframes and renaming the columns
merged = pd.concat([csvFile, csvFileOriginal], axis=1)
merged.columns = ['Asteeringangle', 'Atimestamp', 'Esteeringangle', 'Etimestamp', 'Lower', 'Upper']

# Calculating the equality of steering angle on the two files based on index (row by row), NOT A TIMESTAMP COMPARISON
comparisonSteering(merged)

# plotting
plotLineGraphComp(csvFile, csvFileOriginal)
