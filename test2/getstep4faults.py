# check for unique faults

f1 = open('tb3.xa/tb3.xa.FM001.dangerous_undetected','r')
f2 = open('tb3.xa/tb3.xa.FM002.dangerous_undetected','r')
f3 = open('step4.faults','w')

l1 = f1.readlines()
l2 = f2.readlines()

f1.close()
f2.close()

#print(l1[0])

fm1smfaultlist=[]
fm2smfaultlist=[]

for i in l1:
    f = i.split(',')[1]
    fm1smfaultlist.append(f)
    #print(f)

for i in l2:
    f = i.split(',')[1]
    fm2smfaultlist.append(f)
    #print(f)

print(fm1smfaultlist[0])
print(fm2smfaultlist[0])


cnt=len(fm1smfaultlist)
ufaults=[]
for i in fm2smfaultlist:
    if i in fm1smfaultlist:
        print('duplicate:')
        print(i)                       
    else:
        cnt+=1
        ufaults.append(i)

print('Number Unique Faults = %d'%cnt)
print(len(ufaults))

for i in fm1smfaultlist:
    f3.write(i[3:])
for i in ufaults:
    f3.write(i[3:])


f3.close()

