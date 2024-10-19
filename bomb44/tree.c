typedef struct SecretStruct{
    int num;
    struct SecretStruct* l;
    struct SecretStruct* r;
} TreeNode;

int fun7(TreeNode* a, int x) {
    if (a == 0) {
        return -1;
    }

    if (a->num > x) {
        return 2 * fun7(a->l, x);
    } else if (a->num < x) {
        return 2 * fun7(a->r, x) + 1;
    } else {
        return 0;
    }
}