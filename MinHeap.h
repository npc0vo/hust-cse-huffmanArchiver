#pragma once
#include <huffman.h>
class MinHeap {
private:
    std::vector<node*> heap;

    // ���µ�����  
    void heapifyDown(int index) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        //���Ƶ����ͬ��ȡASCII���С���ַ�
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

    // ���ϵ�����
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            //���Ƶ����ͬ��ȡASCII���С���ַ�
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

    // ����ڵ�
    void insert(node* n) {
        heap.push_back(n);
        heapifyUp(heap.size() - 1);
    }

    // ��ȡ��С�ڵ�
    node* extractMin() {
        if (heap.empty())
            return nullptr;

        node* minNode = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        heapifyDown(0);
        return minNode;
    }

    // ��ȡ�Ѵ�С
    size_t size() const {
        return heap.size();
    }


};