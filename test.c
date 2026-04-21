#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// ===================================================
// 🎯 终点 (Sink)：底层脆弱函数
// 触发条件：传入的 input 必须为 NULL
// ===================================================
void vulnerable_sink(char *input) {
    // 【漏洞点】：没有任何 if (input != NULL) 的校验，直接扔进 strlen
    // 如果 input 为 NULL，strlen 内部会发生段错误 (Segmentation fault)
    size_t len = strlen(input); 
    printf("Processed data length: %zu\n", len);
}

// ===================================================
// 🌿 分支 A：绝对安全的死胡同 (传入死常量) -> 应该被剪枝
// ===================================================
void handle_internal_ping() {
    // LLM 推演："pong" 是有效的字符串常量，永远不可能是 NULL。
    // 约束冲突，果断剪枝放弃！
    vulnerable_sink("pong");
}

// ===================================================
// 🌿 分支 B：被安全校验拦截的半死路 -> 应该被剪枝
// ===================================================
void handle_auth_token(char *token) {
    // LLM 推演：如果外部传入了 NULL（满足炸弹条件），
    // 这里的 if (token == NULL) 会直接 return，导致无法到达 Sink。
    // 约束被阻断，果断剪枝放弃！
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
    // LLM 推演：没有任何判空逻辑！
    // 如果外部传入的 config_val 是 NULL，它将被原封不动地传给 Sink。
    // 这正是我们要找的完美利用链！
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
    // 【关键点】：如果用户不传第二个参数，payload 就是 NULL！
    char *payload = (argc > 2) ? argv[2] : NULL; 

    if (strcmp(route, "ping") == 0) {
        handle_internal_ping();               // 走分支 A
    } else if (strcmp(route, "auth") == 0) {
        handle_auth_token(payload);           // 走分支 B
    } else if (strcmp(route, "config") == 0) {
        parse_malicious_config(payload);      // 走分支 C
    }
    
    return 0;
}