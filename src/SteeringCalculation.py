# Imports
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt



def testGraph(ourFile, originalFile):
    originalFile['Lower'] = originalFile['groundSteering'] / 2.0
    originalFile['Upper'] = originalFile['groundSteering'] * 1.5

    #needs the length of the number of rows in the file
    x = np.arange(1,816,1)

    y1 = originalFile['groundSteering'] + 1
    y2 = originalFile['Lower'] +1
    y3 = originalFile['Upper'] +1

    plt.plot(x, y1,'b',  label = "Actual")
    plt.plot(x, y2,  label = "Lower threshold")
    plt.plot(x, y3,  label = "Upper threshold")

    plt.fill_between(x, y1, y2, where=y1 < y2, color='#539ecd')
    plt.fill_between(x, y1, y2, where=y1 > y2, color='#539ecd')

    plt.fill_between(x, y1, y2, where=y1 > y3, color='#539ecd')
    plt.fill_between(x, y1, y2, where=y1 < y3, color='#539ecd')

    plt.grid()
    plt.legend()
    plt.title('Showing areas of deviation', fontsize=10)
    plt.show()



def comparisonSteering(mergedFile):

    mergedFile['InRange'] = (mergedFile['Lower'] < mergedFile['Esteeringangle']) & (mergedFile['Upper'] > mergedFile['Esteeringangle'])
    #mergedFile['InRange'] = (mergedFile.Atimestamp == mergedFile.Etimestamp) & (mergedFile.Esteeringangle == mergedFile.Asteeringangle)
    print("This is the equality")
    print(mergedFile.InRange)


def plotLineGraphComp(ourFile, originalFile):
    plt.plot(originalFile['groundSteering'], label = "Expected")
    plt.plot(ourFile['groundSteering'], label = "Actual")

    plt.ylabel('Steering Angel 0.1 = 10 degrees')
    plt.xlabel('N timestamps')
    plt.title('Comparison of steering angel')
    plt.legend()

    plt.show()



def plotLineGraphULT(ourFile, originalFile):

    plt.plot(originalFile['Lower'], label = "lower threshold")
    plt.plot(originalFile['Upper'], label = "upper threshold")
    plt.plot((ourFile['groundSteering']), label = "Actual")
    #plt.plot(originalFile['groundSteering'], label = "Expected")

    plt.ylabel('Steering Angel 0.1 = 10 degrees')
    plt.xlabel('N timestamps')
    plt.title(' Steering angel with upper/lower thresholds')
    plt.legend()

    plt.show()


def sliceAndCalculateTimestamp(df):

    # converting sampleTimeStap columns to string to then concatenate them with a comma
    df['sampleTimeStamp.seconds'] = df['sampleTimeStamp.seconds'].astype(str)
    df['sampleTimeStamp.microseconds'] = df['sampleTimeStamp.microseconds'].astype(str)
    df['Timestamp'] = df['sampleTimeStamp.seconds'] + df['sampleTimeStamp.microseconds']
    df['Lower'] = df['groundSteering'] / 2.0
    df['Upper'] = df['groundSteering'] * 1.5

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
    df['Timestamp'] = df['Timestamp'].astype('int64')

    # returing the dataframe
    return df

# importing csvs
csvFile = pd.read_csv("calculated-steering-angle.csv", sep = ";")
csvFileOriginal = pd.read_csv("1452.csv", sep = ";")

# Slicing and reformatting into dataframes
csvFile = sliceAndCalculateTimestampImportedFile(csvFile)
csvFileOriginal = sliceAndCalculateTimestamp(csvFileOriginal)

# dropping n last rows to have equal number of rows to make comparisons
csvFile.drop(csvFile.tail(7).index,inplace=True)

# Merging the dataframes and renaming the columns
merged = pd.concat([csvFile, csvFileOriginal], axis=1)
merged.columns = ['Asteeringangle', 'Atimestamp', 'Esteeringangle', 'Etimestamp', 'Lower', 'Upper']

# Calculating the equality of steering angle on the two files based on index (row by row), NOT A TIMESTAMP COMPARISON
comparisonSteering(merged)

# printing
#print("Merged----------------------------------------------------------------------------------")
#print(merged.info)
#print(merged.head(10))
#print(merged.columns)
#
#print("Actual----------------------------------------------------------------------------------")
#print(csvFile.info)
#print(csvFile.dtypes)
#print(csvFile.head(20))
#
#print("Expected-----------------------------------------------------------------------------------")
#print(csvFileOriginal.info)
#print(csvFileOriginal.dtypes)
#print(csvFileOriginal.head(20))


# plotting
plotLineGraphComp(csvFile, csvFileOriginal)
plotLineGraphULT(csvFile, csvFileOriginal)

#testGraph(merged)


