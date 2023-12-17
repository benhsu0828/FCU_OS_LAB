#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3
#define MAX_PAGES 30

typedef struct {
    int frames[MAX_FRAMES];
    int order[MAX_FRAMES];
    int changeOrder[MAX_FRAMES];
    int count;
    int pageFaults;
} LFUCache;

LFUCache createLRUCache() {
    LFUCache cache;
    cache.count = 0;
    cache.pageFaults = 0;
    for (int i = 0; i < MAX_FRAMES; ++i) {
        cache.frames[i] = -1;
        cache.order[i] = -1;
        cache.changeOrder[i] = 0;
    }
    return cache;
}

void displayCache(LFUCache *cache) {
    for (int i = 0; i < cache->count; ++i) {
        printf("%d ", cache->frames[i]);
    }
    printf("\n");
}

void accessPage(LFUCache *cache, int page) {
    int index = -1;
    
    // 檢查 page 是否已經在 cache 中
    for (int i = 0; i < MAX_FRAMES; ++i) {
        if (cache->frames[i] == page) {
            index = i;
            break;
        }
        cache->changeOrder[i]++;
    }
    if (index != -1) {
        // 如果 page 已經在 cache 中，更新 access order
        cache->order[index]++;
    } else {
        // 如果 page 不在 cache 中，將 page 加入 cache
        if (cache->count < MAX_FRAMES) {
            // 如果 cache 還沒滿，直接將 page 加入 cache 
            cache->frames[cache->count] = page;
            cache->order[cache->count] = 0;
            cache->changeOrder[cache->count] = 0;
            cache->count++;
            cache->pageFaults++;
        } else {
            // 如果 cache 已經滿了，找到 access order 最小的 page，將其替換掉
            int minOrderIndex = 0;
            for (int i = 0; i < MAX_FRAMES; ++i) {
                if (cache->order[i] < cache->order[minOrderIndex]) {
                    minOrderIndex = i;
                }else if(cache->order[i] == cache->order[minOrderIndex]){//如果order相同，則比較change order，把最舊的替換掉
                    if(cache->changeOrder[i] > cache->changeOrder[minOrderIndex]){
                        minOrderIndex = i;
                    }
                }
            }

            // 替換掉 access order 最小的 page
            cache->frames[minOrderIndex] = page;
            cache->order[minOrderIndex] = 0;
            cache->changeOrder[minOrderIndex] = 0;
            cache->pageFaults++;
        }
    }
    printf("Accessing page %d: \n", page);
    // printf("Current cache order: \n");
    // for(int i = 0; i < MAX_FRAMES; i++){
    //     printf("page: %d order: %d\n", cache->frames[i],cache->order[i]);
    // }
    // printf("Current pageFaults: %d\n", cache->pageFaults);
}

int main() {
    LFUCache cache = createLRUCache();
    int pages;

    // 輸入 page 的數量
    printf("Enter the number of pages: (up to %d): ", MAX_PAGES);
    scanf("%d", &pages);

    // 檢查輸入的 page 數量是否合法
    if (pages <= 0 || pages > MAX_PAGES) {
        printf("Invalid input. Please enter pages (1-%d).\n", MAX_PAGES);
        return 1;
    }

    int pageReferences[MAX_PAGES];

    // 輸入 page reference
    printf("Enter page reference (separated by spaces): \n");
    for (int i = 0; i < pages; ++i) {
        scanf("%d", &pageReferences[i]);
    }

    for (int i = 0; i < pages; ++i) {
        accessPage(&cache, pageReferences[i]);
        displayCache(&cache);
    }

    printf("Total Page faults: %d\n", cache.pageFaults);

    return 0;
}