#!/usr/bin/env python3
"""mkopus.py: append Opus batches (10 near-miss drafts each) to the ledger."""
import json, glob, os
OUT='build/overnight/'
L=json.load(open(OUT+'ledger.json'))
unit_of={}; matched=set()
for b in L['batches']:
    for a in b['addrs']:
        unit_of.setdefault(a,[])
        r=b['results'].get(a)
        if r and r.get('outcome')=='match': matched.add(a)
    for a in b['addrs']:
        pass
# map addr -> unit via ctx first line
import re
def unit(a):
    t=open(OUT+f'ctx/{a}.txt',encoding='utf-8').readline()
    m=re.search(r'unit (\d+)',t); return m.group(1)
drafts=sorted(os.path.basename(p)[:-4] for p in glob.glob(OUT+'drafts/*.cpp'))
todo=[d for d in drafts if d not in matched]
# group by unit so each batch has few units
todo.sort(key=lambda a:(int(unit(a)),a))
n=max(int(x['id'][1:]) for x in L['batches'])+1
n=max(n,300)
for i in range(0,len(todo),10):
    ch=todo[i:i+10]
    us=[]
    for a in ch:
        u=unit(a)
        if u not in us: us.append(u)
    L['batches'].append({'id':f'o{n:04d}','units':us,'named':True,'part':{},'addrs':ch,'status':'queued','results':{},'opus':1})
    n+=1
json.dump(L,open(OUT+'ledger.json','w'),indent=1)
print(len(todo),'drafts ->',(len(todo)+9)//10,'batches')
