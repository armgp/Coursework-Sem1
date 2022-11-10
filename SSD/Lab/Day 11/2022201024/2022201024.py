import csv
data=[]
with open('lab_11_data.csv', 'r') as csvFile:
	csvReader = csv.reader(csvFile)
	
	for line in csvReader:
		row = []
		for i in range (7):
			row.append(line[i])
		data.append(row)
csvFile.close()

result = list(filter(lambda x: (float(x[6]) >= float(-3)), data[1:])) 

open_list = list(map(lambda r: float(r[1].replace(",", "")), result))
high_list = list(map(lambda r: float(r[2].replace(",", "")), result))
low_list = list(map(lambda r: float(r[3].replace(",", "")), result))

ohl_list = [open_list, high_list, low_list]
avg_list = list(map(lambda lst: sum(lst)/float(len(lst)), ohl_list))
print(avg_list)

with open('avg_output.txt', 'w') as f:
	f.write("Open-Average = {}\n".format(avg_list[0]))
	f.write("High-Average = {}\n".format(avg_list[1]))
	f.write("Low-Average = {}\n".format(avg_list[2]))
f.close()

charachter = input("Enter a charachter from A-Z: ")
companies = list(filter(lambda x: x[0][0] == charachter, result)) 
with open('stock_output.txt', 'w') as f:
	for company in companies:
		f.write(str(company).replace(",", " ").replace("[", "").replace("]", "\n").replace("'", ""))
f.close()
print(companies)
