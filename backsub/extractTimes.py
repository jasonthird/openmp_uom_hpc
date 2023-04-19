import pandas as pd
datafile = open("benchmark.txt", "r")

# Read the file
text = datafile.read()

dataPD= pd.DataFrame(columns=['threads'])
dataPD['threads'] = [1,2,4,8,16]

program = ""
programName= ""
threads = ""
for (i, line) in enumerate(text.splitlines()):
    # find and get the program name
    if "running" in line:
        program = line.split(" ")[1]
        program = program.split("-")[0]
        #set threads to 1 if program contains seq
        if "seq" in program:
            threads = 1
    # find the size of the array
    if "size of array" in line:
        #use regex to get the size
        size = line.split(" ")[3]
        size = size.split("-")[0]
        programName = program + "_" + size
        #add the program name to the dataframe as a column
        dataPD[programName] = 0
        dataPD[programName + " total"] = 0

    # find the number of threads
    if "number of threads" in line:
        threads = line.split(" ")[3]
        threads = threads.split(" ")[0]
    
    # find the time taken
    if "Time taken" in line:
        time = line.split(" ")[2]
        time = time.split(" ")[0]
        # add the time to the dataframe
        dataPD.loc[dataPD['threads'] == int(threads), programName] = time
    # find the total time
    if "Total Time" in line:
        time = line.split(" ")[2]
        time = time.split(" ")[0]
        # add the time to the dataframe
        dataPD.loc[dataPD['threads'] == int(threads), programName + " total"] = time
        

# calculate the thread speedup

for (i, row) in dataPD.iterrows():
    for (j, col) in row.iteritems():
        if "seq" in j:
            continue
        if "threads" in j:
            continue
        #get time for thread 1
        baseline = dataPD.loc[dataPD['threads'] == 1, j].values[0]
        baseline = float(baseline)
        #get time for current thread
        time = dataPD.loc[dataPD['threads'] == int(row['threads']), j].values[0]
        time = float(time)
        #calculate speedup
        if time == 0:
            speedup = 0
        else:
            speedup = baseline/time
        #add speedup to dataframe
        dataPD.loc[dataPD['threads'] == int(row['threads']), j + " speedup"] = speedup





#save
dataPD.to_excel("benchmark.xlsx")

