class HeapNode{
    /**
     * @type {HeapNode}
     */
    left = undefined;
    /**
     * @type {HeapNode}
     */
    right = undefined;
    /**
     * @type {HeapNode}
     */
    top = undefined;
    /**
     * 
     * @param {Any} value 
     */
    constructor(value) {
        this.value = value;   
    }

    get dump(){
        return {
            value:this.value,
            left:this.left?.dump,
            right:this.right?.dump
        };
    }
}

export class Heap{
    /**
     * @type {HeapNode}
     */
    #entry = undefined;
    #length = 0;
    /**
     * 
     * @param {Function} compare the relation of bottom->top
     */
    constructor(compare) {
        /**
         * 
         * @param {HeapNode} n1 
         * @param {HeapNode} n2 
         * @returns {Boolean}
         */
        this.#compare = (n1,n2)=>{
            return compare(n1.value,n2.value);
        };
    }

    /**
     * 
     * @param {Any} value 
     */
    add(value){
        let newNode = new HeapNode(value);
        if(this.#entry === undefined){
            this.#entry = newNode;
            this.#length ++;
            return;
        }
        let {node,branch} = this.#getTop(++this.#length);
        node[branch] = newNode;
        newNode.top = node;
        this.#adjustBackward(newNode);
    }

    /**
     * @returns {Any}
     */
    get(){
        return this.#entry?.value;
    }

    /**
     * @returns {Any}
     */
    pop(){
        const result = this.#entry?.value;
        // TODO: remove entry
        if(this.#length === 1){
            this.#length --;
            this.#entry = undefined;
        }else if(this.#length > 1){
            let {node,branch} = this.#getTop(this.#length);
            this.#entry.value = node[branch].value;
            node[branch] = undefined;
            this.#length --;
            this.#adjustForeward(this.#entry);
        }
        return result;
    }

    #compare = ()=>{}

    /**
     * 
     * @param {HeapNode} node the top node  
     */
    #adjustForeward(node){
        let targetNode = undefined;
        if(node.left !== undefined && node.right === undefined){
            if(this.#compare(node,node.left)){
                targetNode = node.left;
            }
        }else if(node.left === undefined && node.right !== undefined){
            if(this.#compare(node.node.right)){
                targetNode = node.right;
            }
        }else if(node.left !== undefined && node.right !== undefined){
            let lessNode = undefined;
            if(this.#compare(node.left,node.right)){
                lessNode = node.right;
            }else{
                lessNode = node.left;
            }
            if(this.#compare(node,lessNode)){
                targetNode = lessNode;
            }
        }
        if(targetNode !== undefined){
            // exchange value instead of adjusting connection
            let tempValue = node.value;
            node.value = targetNode.value;
            targetNode.value = tempValue;
            this.#adjustForeward(targetNode);
        }
    }

    /**
     * 
     * @param {HeapNode} node the bottom node 
     */
    #adjustBackward(node){
        if(node.top === undefined){
            return;
        }
        if(this.#compare(node.top,node)){
            // exchange value instead of adjusting connection
            let tempValue = node.value;
            node.value = node.top.value;
            node.top.value = tempValue;
            this.#adjustBackward(node.top);
        }
    }

    /**
     * 
     * @param {Number} n starts at 1
     * @returns {{
     *      node:HeapNode,
     *      branch:String
     * }} 
     */
    #getTop(n){
        if(n===1){
            return undefined;
        }
        let node = this.#entry;
        const layers = 32 - Math.clz32(n);
        for(let i=layers-2;i>0;i--)
        {
            if(((n>>>i)&1)===0){
                node = node?.left;
            }else{
                node = node?.right;
            }
        }
        return {node,branch:n&1?'right':'left'};
    }

    get dump(){
        return this.#entry?.dump;
    }

    get length(){
        return this.#length;
    }
}




