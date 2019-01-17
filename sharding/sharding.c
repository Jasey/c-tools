
#include "sharding.h"


int main(int argc, char const *argv[])
{
    /* code */

    const char* baseTableName = argv[1];
    if (baseTableName == NULL)
    {
        printf("please enter base table name\n");
        return 0;
    }
    
    if (argv[2] == NULL)
    {
        printf("please enter form index\n");
        return 0;
    }

    if (argv[3] == NULL)
    {
        printf("please enter to index\n");
        return 0;
    }

    int from = atoi(argv[2]);
    
    int to = atoi(argv[3]);

    initHashTable(baseTableName, from, to);

    printf("> ");

    char input[100];
    scanf("%s", input);
    while(strcmp(input, "quit") != 0)
    {
        if (strlen(input) >= 100)
        {
            printf("sharding id is too long!\n");
        }
        else if (strcmp(input, "--help") == 0)
        {
            printf ("\"quit\" to exit!\n");
        }
        else if (is_number(input) == 0)
        {
            printf("sharding id is not number!\n");
        }
        else
        {
            //find suffix
            uint32_t crc = 0xffffffff;
            
            crc = make_crc(crc, (unsigned char*)input, strlen(input));
            //showTree();
            const char * suffix = ceilingEntry(crc);
            printf("select * from %s%s;\n",baseTableName, suffix);
        }
        printf("> ");
        memset(input, 0, sizeof(input));
        scanf("%s", input);
    }  

    return 0;
}

const char* ceilingEntry(long hashKey)
{
    //printf("des crc %ld\n", hashKey);
    node* pNode = tableNodes;
    while(pNode != NULL)
    {
        long cmp = hashKey - pNode->hashNode;
        if (cmp < 0)
        {
            if (pNode->left != NULL)
            {
                pNode = pNode->left;
            }else
            {
                return pNode->suffix;
            }
        }else if (cmp > 0)
        {
            if (pNode->right != NULL)
            {
                pNode = pNode->right;
            }else
            {   
                node* parent = pNode->parent;
                node* ch = pNode;
                while(parent != NULL && ch == parent->right)
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
        }else
        {
            return pNode->suffix;
        }
    }
    return tableNodes->suffix;
}

int initHashTable(const char* baseTableName, int from, int to)
{
    
    char buf[1024];
    char actualTableSuffix[10];
    char virtualTableSuffix[10];

    for (int i = from; i <= to; i++)
    {
        memset(actualTableSuffix, 0, sizeof(actualTableSuffix));
        sprintf(actualTableSuffix, "%d", i);
        for (int j = 0; j < virtualCount; j++)
        {
            memset((void*)buf, 0, sizeof(buf));
            strcpy(buf, baseTableName);
            strcat(buf, actualTableSuffix);
            strcat(buf, "_vir");
            memset(virtualTableSuffix, 0, sizeof(virtualTableSuffix));
            sprintf(virtualTableSuffix, "%d", j);
            strcat(buf, virtualTableSuffix);
            uint32_t crc = 0xffffffff;
            crc = make_crc(crc, (unsigned char*)buf, strlen(buf));
            //printf("%s, crc : %ld\n",buf, crc);
            node* pNode = (node*)malloc(sizeof(node));
            memset(pNode, 0, sizeof(node));
            strcpy(pNode->suffix, actualTableSuffix);
            pNode->hashNode = crc;
            pNode->parent = NULL;
            pNode->left = NULL;
            pNode->right = NULL;

            if (tableNodes == NULL)
            {
                tableNodes = pNode;
            }else
            {
                addNodeToTree(tableNodes, pNode);
            }   
        }
    }
    return 1;
}

int addNodeToTree(node* root, node *p)
{
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
        }else
        {
            return addNodeToTree(root->left, p);
        }
    }else
    {
         
        if (root->right == NULL)
        {
            
            root->right = p;
            p->parent = root;
            return 1;
        }else
        {
            return addNodeToTree(root->right, p);
        }
    }
}

uint32_t make_crc(uint32_t crc, unsigned char *string, uint32_t size)
{
 
	while(size--)
		crc = (crc >> 8)^(crc32_table[(crc ^ *string++)&0xff]);
 
	return crc ^ 0xffffffff;
}


void showTree()
{
    node* p = tableNodes;
    if (p == NULL)
    {
        printf ("tree is null\n");
        return;
    }
    printf("node : %ld, ", p->hashNode);
    if (p->left != NULL)
    {
        printf("left : %ld, ", p->left->hashNode);
    }else
    {
        printf("left : null, ");
    }
    if (p->right != NULL)
    {
        printf("right : %ld\n", p->right->hashNode);
    }else
    {
        printf("right : null\n");
    }
}


int is_number(char * s)
{
	int base = 10;
	char *ptr;
	int type = 0;
 
	if (s==NULL) return 0;
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