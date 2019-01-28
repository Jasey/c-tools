
#include "sharding.h"

int algorithm = 0;
const char *baseTableName = NULL;
int from = 0;
int to = 199;
const char *fileName = NULL;

node *tableNodes;

int main(int argc, char const *argv[])
{
    /* code */

    if (argv[1] != NULL)
    {
        if (strcmp(argv[1], A_CRC) == 0)
        {
            algorithm = 0;

            if (argv[2] != NULL)
            {
                baseTableName = argv[2];
            }

            if (argv[3] != NULL)
            {
                from = atoi(argv[3]);
            }

            if (argv[4] != NULL)
            {
                to = atoi(argv[4]);
            }
        }
        else if (strcmp(argv[1], A_FILE) == 0)
        {
            algorithm = 1;
            if (argv[2] != NULL)
            {
                fileName = argv[2];
            }
        }
    }

    initHashTable(algorithm);

    printf("> ");

    char input[100];
    scanf("%s", input);
    while (strcmp(input, "quit") != 0)
    {
        if (strlen(input) >= 100)
        {
            printf("sharding id is too long!\n");
        }
        else if (strcmp(input, "--help") == 0)
        {
            help();
        }
        else if (strcmp(input, EDIT) == 0)
        {
             edit();
        }
        else if (strcmp(input, INFO) == 0)
        {
            info();
        }
        
        else if (is_number(input) == 0)
        {
            printf("sharding id is not number!\n");
        }
        else
        {
            //find suffix
            uint32_t crc = 0xffffffff;

            crc = make_crc(crc, (unsigned char *)input, strlen(input));
            //showTree();
            const char *suffix = ceilingEntry(crc);
            switch (algorithm)
            {
                case CRC_TYPE:
                    printf("select * from %s%s;\n", baseTableName, suffix);
                    break;
                case FILE_TYPE:
                    printf("select * from %s;\n", suffix);
                    break;
            
                default:
                    printf("select * from %s%s;\n", baseTableName, suffix);
                    break;
            }
            
        }
        printf("> ");
        memset(input, 0, sizeof(input));
        scanf("%s", input);
    }

    return 0;
}

const char *ceilingEntry(long hashKey)
{
    //printf("des crc %ld\n", hashKey);
    node *pNode = tableNodes;
    while (pNode != NULL)
    {
        long cmp = hashKey - pNode->hashNode;
        if (cmp < 0)
        {
            if (pNode->left != NULL)
            {
                pNode = pNode->left;
            }
            else
            {
                return pNode->suffix;
            }
        }
        else if (cmp > 0)
        {
            if (pNode->right != NULL)
            {
                pNode = pNode->right;
            }
            else
            {
                node *parent = pNode->parent;
                node *ch = pNode;
                while (parent != NULL && ch == parent->right)
                {
                    ch = parent;
                    parent = parent->parent;
                }
                if (parent == NULL)
                {
                    return tableNodes->suffix;
                }
                return parent->suffix;
            }
        }
        else
        {
            return pNode->suffix;
        }
    }
    return tableNodes->suffix;
}

int initHashTable(int type)
{
    switch (type)
    {
    case CRC_TYPE:
    {
        if (baseTableName == NULL)
        {
            printf("base table name is null\n");
            return help();
        }
        return initCrc(baseTableName, from, to);
    }

    case FILE_TYPE:
    {
        if (fileName == NULL)
        {
            return help();
        }
        return initFile(fileName);
    }
    default:
    {
        printf("unsopported algorithm\n");
        return help();
    }
    }
}

int addNodeToTree(node *root, node *p)
{

    if (tableNodes == NULL)
    {
        tableNodes = p;
        return 1;
    }

    if (root->hashNode == p->hashNode)
    {
        strcpy(root->suffix, p->suffix);
        return 1;
    }
    if (root->hashNode > p->hashNode)
    {

        if (root->left == NULL)
        {
            root->left = p;
            p->parent = root;
            return 1;
        }
        else
        {
            return addNodeToTree(root->left, p);
        }
    }
    else
    {

        if (root->right == NULL)
        {

            root->right = p;
            p->parent = root;
            return 1;
        }
        else
        {
            return addNodeToTree(root->right, p);
        }
    }
}

uint32_t make_crc(uint32_t crc, unsigned char *string, uint32_t size)
{

    while (size--)
        crc = (crc >> 8) ^ (crc32_table[(crc ^ *string++) & 0xff]);

    return crc ^ 0xffffffff;
}

void showTree()
{
    node *p = tableNodes;
    if (p == NULL)
    {
        printf("tree is null\n");
        return;
    }
    printf("node : %ld, ", p->hashNode);
    if (p->left != NULL)
    {
        printf("left : %ld, ", p->left->hashNode);
    }
    else
    {
        printf("left : null, ");
    }
    if (p->right != NULL)
    {
        printf("right : %ld\n", p->right->hashNode);
    }
    else
    {
        printf("right : null\n");
    }
}

int is_number(char *s)
{
    int base = 10;
    char *ptr;
    int type = 0;

    if (s == NULL)
        return 0;
    if (s[0] <= '0')
    {
        return 0;
    }
    for (int i = 0; i < strlen(s); i++)
    {
        char c = s[i];
        if (c < '0' || c > '9')
        {
            return 0;
        }
    }
    return 1;
}

int help()
{
    printf("\"--info\" : show your config\n");
    printf("\"--edit\" : redit your config\n");
    printf("\"--help\" : more help\n");
    printf("\"quit\" : exit\n");
    return 1;
}

int initCrc(const char *baseTableName, int from, int to)
{
    char buf[1024];
    char actualTableSuffix[MAX_SUFFIX_LEN];
    char virtualTableSuffix[MAX_SUFFIX_LEN];

    for (int i = from; i <= to; i++)
    {
        memset(actualTableSuffix, 0, sizeof(actualTableSuffix));
        sprintf(actualTableSuffix, "%d", i);
        for (int j = 0; j < virtualCount; j++)
        {
            memset((void *)buf, 0, sizeof(buf));
            strcpy(buf, baseTableName);
            strcat(buf, actualTableSuffix);
            strcat(buf, "_vir");
            memset(virtualTableSuffix, 0, sizeof(virtualTableSuffix));
            sprintf(virtualTableSuffix, "%d", j);
            strcat(buf, virtualTableSuffix);
            uint32_t crc = 0xffffffff;
            crc = make_crc(crc, (unsigned char *)buf, strlen(buf));
            //printf("%s, crc : %ld\n",buf, crc);
            node *pNode = (node *)malloc(sizeof(node));
            memset(pNode, 0, sizeof(node));
            strcpy(pNode->suffix, actualTableSuffix);
            pNode->hashNode = crc;
            pNode->parent = NULL;
            pNode->left = NULL;
            pNode->right = NULL;

            addNodeToTree(tableNodes, pNode);
        }
    }
    return 1;
}

int initFile(const char *fileName)
{
    FILE *fd = fopen(fileName, "r");
    if (fd == NULL)
    {
        printf("can't find file :%s in current folder\n", fileName);
        help();
        return 1;
    }
    long hashNode;
    char suffix[MAX_SUFFIX_LEN];
    char line[MAX_LINE_LEN];
    memset(suffix, 0, MAX_SUFFIX_LEN);

    while (!feof(fd))
    {
        fgets(line, MAX_LINE_LEN, fd);

        parseNode(line, suffix, &hashNode);

        node *pNode = (node *)malloc(sizeof(node));
        memset(pNode, 0, sizeof(node));

        strcpy(pNode->suffix, suffix);
        pNode->hashNode = hashNode;
        pNode->parent = NULL;
        pNode->left = NULL;
        pNode->right = NULL;
        addNodeToTree(tableNodes, pNode);

        memset(suffix, 0, MAX_SUFFIX_LEN);
    }

    fclose(fd);

    return 1;
}

int parseNode(const char* line, char *suffix, long* pHash)
{
    char hashNodeString [MAX_SUFFIX_LEN];
    memset(hashNodeString, 0, MAX_SUFFIX_LEN);
    char suffixString [MAX_LINE_LEN];
    memset(suffixString, 0, MAX_SUFFIX_LEN);

    int index = 0;
    int elIndex = 0;
    for (int i = 0; i < strlen(line); i++)
    {
        if (line[i] == ',')
        {
            elIndex++;
            index = 0;
            continue;
        }
        
        if (elIndex == 1)
        {
            hashNodeString[index] = line[i];
        }else if (elIndex == 2)
        {
            suffixString[index] = line[i];
        }

        index ++;
    }
    hashNodeString[strlen(hashNodeString)-1] = '\0';
    suffixString[strlen(suffixString)-1] = '\0';

    *pHash = atol(&hashNodeString[1]);
    strcpy(suffix, &suffixString[1]);
    return 1;
}


void edit()
{
    tableNodes = NULL;

    char a[10];

    printf("algorithm (%s | %s): ", A_CRC, A_FILE);
    scanf("%s", a);
    if (strcmp(a, A_CRC) == 0)
    {
        algorithm = 0;
    }
    else if (strcmp(a, A_FILE) == 0)
    {
        algorithm = 1;
    }
    
    switch(algorithm)
    {
        case CRC_TYPE:
        {
            printf("baseTableName : ");
            baseTableName = (char*)malloc(MAX_SUFFIX_LEN);
            memset((void*)baseTableName, 0, MAX_SUFFIX_LEN);

            scanf("%s", (char *)baseTableName);


            printf("from : ");
            scanf("%d", &from);

            printf("to : ");
            scanf("%d", &to);

            initCrc(baseTableName, from, to);
            break;

        }
        case FILE_TYPE:
        {
            printf("fileName : ");

            fileName = (char*)malloc(MAX_SUFFIX_LEN);
            memset((void *)fileName, 0, MAX_SUFFIX_LEN);

            scanf("%s", (char *)fileName);

            initFile(fileName);
            break;
        }
    }
}

void info()
{
    printf("====config====\n");
    switch(algorithm)
    {
        case CRC_TYPE:
        {
            printf("algorithm : %s\n", A_CRC);
            printf("baseTableName : %s\n", baseTableName);
            printf("from : %d\n", from);
            printf("to : %d\n", to);
            break;
        }
        case FILE_TYPE:
        {
            printf("algorithm : %s\n", A_FILE);
            printf("fileName : %s\n", fileName);
            break;
        }
    }
    printf("==============\n");
}