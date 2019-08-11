arr = [input().split("|") for i in range(15)]

import re
res = []
for i in arr:
    res.append([])
    res[-1].append(i[0].strip())
    for j in range(1, len(i)):
        res[-1].extend(re.split(r"\s",i[j].strip()))

print("                        |", end="")
for i in range(1, 10):
    print(f"      {i}      ", end="|")

print()
print("                        |", end="")
for i in range(1, 10):
    print(f"  H     L    ", end="|")

print()
for i in res:
    print(f"{i[0]:24}", end=",")
    for j in range(1, len(i)):
        if i[j] != "": print(f"{i[j]:6}", end=",")
    print()


########################################################################################
### EXCEL formula ###

# =SUM(B26:B40)
# =SUM(C26:C40)

# =MIN(B42,D42,F42,H42,J42,L42,N42,P42,R42)
# =MIN(C42,E42,G42,I42,K42,M42,O42,Q42,S42)
