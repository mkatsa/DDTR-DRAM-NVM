import os
import csv
import glob
import sys
from operator import itemgetter
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import random 


def plot_pareto_frontier(Xs, Ys, Ns, maxX=False, maxY=False):
	'''Pareto frontier selection process'''
	sorted_list = sorted([[Xs[i], Ys[i], Ns[i]] for i in range(len(Xs))], reverse=maxY)
	pareto_front = [sorted_list[0]]
	pareto_logs = []
	for pair in sorted_list[1:]:
	    if maxY:
	        if pair[1] >= pareto_front[-1][1]:
	            pareto_front.append(pair)
	            pareto_logs.append(pair[2])
	    else:
	        if pair[1] <= pareto_front[-1][1]:
	            pareto_front.append(pair)
	            pareto_logs.append(pair[2])

	fig= plt.figure(figsize=(7,5))	
	plt.scatter(Xs,Ys,color = 'black', s=10)
	pf_X = [pair[0] for pair in pareto_front]
	pf_Y = [pair[1] for pair in pareto_front]
	plt.plot(pf_X, pf_Y, color = 'red')
	plt.xlabel("Memory Accesses")
	plt.ylabel("Memory Footprint(MB)")
	#plt.subplots_adjust(left=0.11, bottom=0.11, right=0.9, top=0.88, wspace=0.2, hspace=0.2)
	plt.savefig(code_root + "/pareto.png")
	plt.savefig(code_root + "/pareto.pdf")
	return pareto_logs

extension = 'out'
all_filenames = [i for i in glob.glob('*.{}'.format(extension))]

for i in range(0, int(sys.argv[1])):
	globals()[f"memory_accesses{i}"] = []
	globals()[f"memory_footprint{i}"] = []

code_root = sys.argv[2]

DDTs=["VECTOR", "SLLLIST", "DLLLIST", "SLLROVINGLIST", "DLLROVINGLIST"]
total=len(DDTs)

for current_DDT in DDTs:
	f=code_root + "/log_"+ current_DDT +".out"
	with open(f) as out_file:
		lineno = 0
		idline = 0
		lines = out_file.readlines()
		for line in lines:
			lineno+=1
			line = line.strip()
			if lineno >= 8:
				lineno += 1
				if line.startswith("total"):
					accesses = int(line.split(" ")[3])
					globals()[f"memory_accesses{idline}"].append(accesses)

				if line.startswith("max"):
					footprint = int(line.split(" ")[3])
					globals()[f"memory_footprint{idline}"].append(footprint)
					idline+=1


comb_num = 0
f = open(code_root +"/log_combinations.out", "w")

ddts_comb=[]
combination_list = []
memory_accesses = []
memory_footprint = []
for key0,a0 in enumerate(memory_accesses0):
	for key1,a1 in enumerate(memory_accesses1):
		for key2,a2 in enumerate(memory_accesses2):
			for key3,a3 in enumerate(memory_accesses3):
				f.write(str(comb_num) + " -> ")

				temp=[]
				if key0 == 0 :
					temp.append("VECTOR")
					f.write("VECTOR ")
				if key0 == 1 :
					temp.append("SLLLIST")	
					f.write("SLLLIST ")
				if key0 == 2 :
					temp.append("DLLLIST")	
					f.write("DLLLIST ")
				if key0 == 3 :
					temp.append("SLLROVINGLIST")	
					f.write("SLLROVINGLIST ")
				if key0 == 4 :
					temp.append("DLLROVINGLIST")	
					f.write("DLLROVINGLIST ")

				if key1 == 0 :
					temp.append("VECTOR")
					f.write("VECTOR ")
				if key1 == 1 :
					temp.append("SLLLIST")	
					f.write("SLLLIST ")
				if key1 == 2 :
					temp.append("DLLLIST")	
					f.write("DLLLIST ")
				if key1 == 3 :
					temp.append("SLLROVINGLIST")	
					f.write("SLLROVINGLIST ")
				if key1 == 4 :
					temp.append("DLLROVINGLIST")	
					f.write("DLLROVINGLIST ")	

				if key2 == 0 :
					temp.append("VECTOR")
					f.write("VECTOR ")
				if key2 == 1 :
					temp.append("SLLLIST")	
					f.write("SLLLIST ")
				if key2 == 2 :
					temp.append("DLLLIST")	
					f.write("DLLLIST ")
				if key2 == 3 :
					temp.append("SLLROVINGLIST")	
					f.write("SLLROVINGLIST ")
				if key2 == 4 :
					temp.append("DLLROVINGLIST")	
					f.write("DLLROVINGLIST ")	

				if key3 == 0 :
					temp.append("VECTOR")
					f.write("VECTOR ")
				if key3 == 1 :
					temp.append("SLLLIST")	
					f.write("SLLLIST ")
				if key3 == 2 :
					temp.append("DLLLIST")	
					f.write("DLLLIST ")
				if key3 == 3 :
					temp.append("SLLROVINGLIST")	
					f.write("SLLROVINGLIST ")
				if key3 == 4 :
					temp.append("DLLROVINGLIST")	
					f.write("DLLROVINGLIST ")	

				f.write("\n")
				ddts_comb.append(temp)
				combination_list.append(comb_num)
				memory_accesses.append(a0+a1+a2+a3)
				memory_footprint.append((memory_footprint0[key0] + memory_footprint1[key1] + memory_footprint2[key2] + memory_footprint3[key3]))#/1048576)
				comb_num+=1

f.close()
#print(ddts_comb)

if os.path.exists(code_root + "/pareto.png"):
		os.remove(code_root + "/pareto.png")
if os.path.exists(code_root + "/pareto.pdf"):
		os.remove(code_root + "/pareto.pdf")

pareto_combinations = []
pareto_combinations = plot_pareto_frontier(memory_accesses, memory_footprint, combination_list)

output_file = code_root +"/pareto_combinations.txt"
if os.path.exists(output_file):
		os.remove(output_file)

with open(output_file, 'w') as outf:
	for point in pareto_combinations:
		to_write = str(point)+ "\n"
		outf.write(to_write)

