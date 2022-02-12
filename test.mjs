import {Heap} from './Heap.mjs';

let heap = new Heap((A,B)=>A.v>B.v);

let a = {v:1};
let b = {v:1};
let c = {v:1};

heap.add(a);
heap.add(b);
heap.add(c);
heap.delete(a);
heap.delete(b);
heap.delete(c);

