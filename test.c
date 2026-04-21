#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void vulnerable_sink(char *input) {
    if (input == NULL) {
        printf("Error: Input pointer is NULL, safely aborting.\n");
        return; 
    }
    size_t len = strlen(input); 
    printf("Processed data length: %zu\n", len);
}


void A() {
    vulnerable_sink("pong");
}

void B(char *token) {
    if (token == NULL) {
        printf("Security Alert: Token cannot be NULL!\n");
        return; 
    }
    vulnerable_sink(token);
}

void C(char *config_val) {
    
    vulnerable_sink(config_val);
}

// ===================================================
// 🚪 顶级路由：模拟外部输入源
// ===================================================
int main(int argc, char **argv) {
    // 模拟真实的命令行解析逻辑
    if (argc < 2) {
        printf("Usage: %s <route> [payload]\n", argv[0]);
        return 1;
    }

    char *route = argv[1];
   
    char *payload = (argc > 2) ? argv[2] : NULL; 

    if (strcmp(route, "ping") == 0) {
        A();               
    } else if (strcmp(route, "auth") == 0) {
        B(payload);           
    } else if (strcmp(route, "config") == 0) {
        C(payload);      
    }
    
    return 0;
}