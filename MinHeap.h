#pragma once
#include <huffman.h>
class MinHeap {
private:
    std::vector<node*> heap;

    // 向下调整堆  
    void heapifyDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        //如果频率相同，取ASCII码较小的字符
        if (left < heap.size() &&
            (heap[left]->freq < heap[smallest]->freq ||
                (heap[left]->freq == heap[smallest]->freq && heap[left]->val < heap[smallest]->val))) {
            smallest = left;
        }
        if (right < heap.size() &&
            (heap[right]->freq < heap[smallest]->freq ||
                (heap[right]->freq == heap[smallest]->freq && heap[right]->val < heap[smallest]->val))) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

    // 向上调整堆
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            //如果频率相同，取ASCII码较小的字符
            if (heap[index]->freq > heap[parent]->freq ||
                (heap[index]->freq == heap[parent]->freq && heap[index]->val >= heap[parent]->val)) {
                break;
            }
            std::swap(heap[index], heap[parent]);
            index = parent;
        }
    }

public:
    MinHeap() = default;

    // 插入节点
    void insert(node* n) {
        heap.push_back(n);
        heapifyUp(heap.size() - 1);
    }

    // 提取最小节点
    node* extractMin() {
        if (heap.empty())
            return nullptr;

        node* minNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return minNode;
    }

    // 获取堆大小
    size_t size() const {
        return heap.size();
    }


};