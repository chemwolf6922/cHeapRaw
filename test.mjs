import {Heap} from './Heap.mjs';
import util from 'util';

let heap = new Heap((A,B)=>A.v>B.v);

let v1 = {v:1};
let v2 = {v:10};
let v3 = {v:2};
let v4 = {v:11};
let v5 = {v:12};
let v6 = {v:4};
let v7 = {v:3};

heap.add(v1);
heap.add(v2);
heap.add(v3);
heap.add(v4);
heap.add(v5);
heap.add(v6);
heap.add(v7);
heap.delete(v4);
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());
console.log(util.inspect(heap.dump,{depth:null}));
console.log(heap.pop());

