import {Heap} from './Heap.mjs';

let heap = new Heap((v1,v2)=>v1>v2);

let startTs = Date.now();
for(let i=0;i<1000000;i++){
    heap.add(Math.random()*1000>>>0);
}
console.log(Date.now()-startTs);
startTs = Date.now();
while(heap.length > 0)
{
    heap.pop();
}
console.log(Date.now()-startTs);
