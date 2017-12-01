#!/usr/bin/python

import csv, sys, glob, os, re

''' 
Script to parse logs and generate param files
Requires input and output directories

python parser.py /course/cs2951w/Fall_2017/willow/data/logs /course/cs2951w/Fall_2017/willow/data/params

'''

parameters = [
				"randseed",
				"generations",
				"varb",
				"varc",
				"vard",
				"varvalue0", #e
				"varvalue1", #f
				"varvalue2", #j
				"varvalue3" #k
			]

def parse(inDir, outDir, parameters):
	if not os.path.isdir(inDir):
		sys.stderr.write("error: %s not a valid in directory" % inDir)
	if not os.path.isdir(outDir):
		os.makedirs(outDir)

	for root, dirs, files in os.walk(inDir):
		for file in files:
			if file.endswith(".txt"):
				path = os.path.join(root, file)
				print("Evaluating: " + path)
				text = open(path, "r")

				index = 0

				params = ""

				csvReader = csv.reader(text)
				for row in csvReader:
					if row:
						if row[0] == "PARAMETER_INFO:":
							params = ", ".join(csvReader.next())
							break
						else:
							index += 1
				outList = []
				for var in parameters:
					subIndex = params.find(var, len(var))
					outList.append(params[subIndex:].rsplit(')')[0].replace("(", "").replace("  ", " ").replace(" ", ", "))

				csvPath = os.path.splitext(os.path.basename(path))[0].replace("log", "params")
				csvPath = os.path.join(outDir, csvPath + ".csv")
				csvText = open(csvPath, "w+")
				for line in outList:
					csvText.write(line + "\n")
				csvText.write("\n")

				csvText.close()
				text.close()
	return 

if __name__ == '__main__':
	if len(sys.argv) != 3:
		sys.stderr.write('usage: ./htasCheckRankings \n')
		sys.exit(1)
	parse(sys.argv[1], sys.argv[2], parameters)
