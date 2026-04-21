#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ===================================================
// 🎯 终点 (Sink)：底层脆弱函数
// 触发条件：传入的 input 必须为 NULL
// ===================================================
void vulnerable_sink(char *input) {
    
    size_t len = strlen(input); 
    printf("Processed data length: %zu\n", len);
}

// ===================================================
// 🌿 分支 A：绝对安全的死胡同 (传入死常量) -> 应该被剪枝
// ===================================================
void handle_internal_ping() {
    vulnerable_sink("pong");
}

// ===================================================
// 🌿 分支 B：被安全校验拦截的半死路 -> 应该被剪枝
// ===================================================
void handle_auth_token(char *token) {
    if (token == NULL) {
        printf("Security Alert: Token cannot be NULL!\n");
        return; 
    }
    vulnerable_sink(token);
}

// ===================================================
// ☠️ 分支 C：毫无防备的畅通毒脉 (Ultimate Source) -> 应该被选中
// ===================================================
void parse_malicious_config(char *config_val) {
    
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
        handle_internal_ping();               
    } else if (strcmp(route, "auth") == 0) {
        handle_auth_token(payload);           
    } else if (strcmp(route, "config") == 0) {
        parse_malicious_config(payload);      
    }
    
    return 0;
}