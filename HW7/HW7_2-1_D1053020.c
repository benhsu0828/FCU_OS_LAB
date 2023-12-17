#include <stdio.h>
#include <stdlib.h>

#define MAX_FRAMES 3
#define MAX_PAGES 30

typedef struct {
    int frames[MAX_FRAMES];
    int order[MAX_FRAMES];
    int count;
    int pageFaults;
} LRUCache;

LRUCache createLRUCache() {
    LRUCache cache;
    cache.count = 0;
    cache.pageFaults = 0;
    for (int i = 0; i < MAX_FRAMES; ++i) {
        cache.frames[i] = -1;
        cache.order[i] = -1;
    }
    return cache;
}

void displayCache(LRUCache *cache) {
    for (int i = 0; i < cache->count; ++i) {
        printf("%d ", cache->frames[i]);
    }
    printf("\n");
}

void accessPage(LRUCache *cache, int page) {
    int index = -1;
    printf("Accessing page %d: ", page);
    // 檢查 page 是否已經在 cache 中
    for (int i = 0; i < MAX_FRAMES; ++i) {
        if (cache->frames[i] == page) {
            index = i;
            break;
        }
    }

    if (index != -1) {
        // 如果 page 已經在 cache 中，更新 access order
        for (int i = 0; i < MAX_FRAMES; ++i) {
            if (i != index) {
                cache->order[i]++;
            }
        }
        cache->order[index] = 0;
    } else {
        // 更新 access order，將每個 page 的 access order 都加 1
        for (int i = 0; i < MAX_FRAMES; ++i) {
                cache->order[i]++;
        }
        // 如果 page 不在 cache 中，將 page 加入 cache
        if (cache->count < MAX_FRAMES) {
            // 如果 cache 還沒滿，直接將 page 加入 cache
            cache->frames[cache->count] = page;
            cache->order[cache->count] = 0;
            cache->count++;
            cache->pageFaults++;
        } else {
            // 如果 cache 已經滿了，找到 access order 最大的 page，將其替換掉
            int maxOrderIndex = 0;
            for (int i = 0; i < MAX_FRAMES; ++i) {
                if (cache->order[i] > cache->order[maxOrderIndex]) {
                    maxOrderIndex = i;
                }
            }

            // 替換掉 access order 最大的 page
            cache->frames[maxOrderIndex] = page;
            cache->order[maxOrderIndex] = 0;
            cache->pageFaults++;
        }
    }
}

int main() {
    LRUCache cache = createLRUCache();
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