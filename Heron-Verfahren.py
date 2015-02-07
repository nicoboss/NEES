D=2.0
w=49.0
i=0
while((D*D-w)*(D*D-w)>0.000000000000000000001):
    D=(D+w/D)/2.0
    i+=1
print(i)
print(D)
