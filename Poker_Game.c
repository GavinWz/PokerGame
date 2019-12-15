#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>

/*算法思想：用二叉排序树存储用户和人机的手牌，*/

typedef struct node{  //二叉排序树节点定义 
    int key;
    struct node *lchild, *rchild;
}*BSTree;

typedef struct Node{  //链表定义 
    int card;
    struct Node *next;
}*LinkList;

void Init_List(LinkList *L);  //链表的初始化 
void Tail_Ins(LinkList L, int ins);  //链表的尾插法创建函数 
void Insert(BSTree *T,int value);  //二叉排序树的插入函数
void Del_Link(LinkList L, int x);  //二叉排序树的删除函数
void Tree_to_List(BSTree root, LinkList one_LinkList);  //将一个二叉排序树中的值存到链表中 
void mknumber(int array[]);  //生成5个随机数并存入array数组中
void get_cards(int user[], int computer[]);  //生成用户和人机的手牌 
void Create(BSTree *user_tree, BSTree *computer_tree);  //建立两个二叉排序树 
void Display_cards(LinkList L);  //显示用户或人机的手牌 
int Computer_against(LinkList computer_list, int card);  //人机的出牌模式 
int computer_show(LinkList L, int card);  //人机在出牌后亮出所出的牌
void computer_del(LinkList L, int card, int x); //人机出牌后删除链表中相应的牌 
int Exist(LinkList L, int card);  //判断用户想要出的牌是否存在 
int User_against(LinkList user_list, int card);  //用户的出牌函数
int Show_card();  //判断用户出的牌是否正确 
void Out_List(LinkList L, int *x);  //出牌时对相应链表中值的删除 
void Welcome(BSTree user_t, BSTree computer_t, LinkList user_l, LinkList computer_l);  //菜单函数 
void Game_begins(LinkList user_list, LinkList computer_list); //游戏开始函数 
void Clear_tree(BSTree root);  //清除二叉排序树中的所有节点 
void Clear_list(LinkList L);  //清除链表中的所有节点 
void Clear(BSTree user_t, BSTree computer_t, LinkList user_l, LinkList computer_l); //清除所有信息 

void Init_List(LinkList *L){
	(*L) = (LinkList)malloc(sizeof(struct Node));
	(*L)->next = NULL;
}

void Tail_Ins(LinkList L, int ins){ //尾插法创建
    LinkList s;
    s = (LinkList)malloc(sizeof(struct Node));
    s->next = L->next;
    s->card = ins;
    L->next = s;
}

void Display_cards(LinkList L){
	LinkList temp = L->next;
    while(temp){
        printf("%d ", temp->card);
        temp = temp->next;
    }
    printf("\n");
}

void Del_Link(LinkList L, int x){
    LinkList pre = L;
    LinkList temp = L->next;
    while(temp && temp->card != x){
		pre = temp;
        temp = temp->next;
    }
    if(temp->card == x){ 
    	pre->next = temp->next;
    	free(temp);
	}
}

void Out_List(LinkList L, int *x){
	LinkList temp = L->next;
	if(temp){
		*x = temp->card;
		L->next = temp->next;
		free(temp);
	}
	else
		printf("No card.\n");
}

void Tree_to_List(BSTree root, LinkList one_LinkList){
	if(root){
		Tree_to_List(root->rchild, one_LinkList);
		Tail_Ins(one_LinkList, root->key);
		Tree_to_List(root->lchild, one_LinkList);
	}
}

void Insert(BSTree *T,int value){  //二叉排序树的插入操作
    BSTree s;
    if((*T) == NULL){
        s = (BSTree)malloc(sizeof(struct node));
        s->key = value;
        s->lchild = NULL;
        s->rchild = NULL;
        *T = s;
    }
    else if(value <= (*T)->key)
        Insert(&((*T)->lchild), value);
    else if(value > (*T)->key)
        Insert(&((*T)->rchild), value);
}

void mknumber(int array[]){ //向一个数组中存储5个随机数
	int i;
	
	for(i = 0; i < 5; i++){
		array[i] = rand() % 41;
		array[i] %= 10;
		if(array[i] == 1){
			i--;
			continue;
		}
		if(array[i] == 0)
			array[i] = 10;
	}
}

void get_cards(int user[], int computer[]){  //
	srand((unsigned)time(NULL)); //以系统时间来设置随机数生成器
	mknumber(user);
	srand((unsigned)time(NULL)*1234);  //为使双方手牌不同而重新设置随机数生成器
	mknumber(computer);
}

void Create(BSTree *user_tree, BSTree *computer_tree){
	/*生成用户和人机手牌的随机数数组，并存储到各自的二叉排序树中*/
    int i;
    *user_tree = NULL;
    *computer_tree = NULL;
    int user[5];
	int computer[5];
    get_cards(user, computer);
    for(i = 0; i < 5; i++){
        Insert(user_tree, user[i]);
        Insert(computer_tree, computer[i]);
    }
}

int computer_show(LinkList L, int card){
	LinkList temp = L->next;
	int judge = 0;
	while(temp->card <= card && temp){
		temp = temp->next;
	}
	if(temp->card > card){
		printf("The computer show %d.\n", temp->card);
		return temp->card;
	}
	else{
		printf("Computer can't follow.\n");
		return 0;
	}
}

void computer_del(LinkList L, int card, int x){
	x = computer_show(L, card);
	if(x)
		Del_Link(L, x);
}

int Exist(LinkList L, int card){
	LinkList temp = L->next;
	while(temp){
		if(temp->card == card){
			return 1;
			break;
		}
		temp = temp->next;
	}
	
	return 0;
}

 void Game_begins(LinkList user_list, LinkList computer_list){
 	int x;
 	int user_card, computer_card;
 	user_card = 1000;
 	Display_cards(user_list);
 	printf("Show your card first.\n");
loop1:scanf("%d", &user_card);
	getchar();
	if(!Exist(user_list, user_card)){
		printf("Please show a card which you have.\n");
 		goto loop1;
 	} 
 	Del_Link(user_list, user_card);
 	while(user_list->next && computer_list->next){
 		computer_card = Computer_against(computer_list, user_card);
		if (!computer_list->next){
			printf("Computer show %d.\n", computer_card);
			break;
		}
			
 		if(computer_card){
			if (!user_list->next)
				break;
 			user_card = User_against(user_list, computer_card);
			
		}
 		else{
			if(!user_list->next)
				break;
			printf("Show your card in the new round.\n");
 			printf("Your cards are: \n");
 			Display_cards(user_list);
			printf("Show your Card:\n");
 	loop2:	scanf("%d", &user_card);
 			if(!Exist(user_list, user_card))
 				goto loop2;
 			Del_Link(user_list, user_card); 
 			continue;
		 }
 		if(user_card)
 			continue;
 		else{
	loop:	if (!computer_list->next)
				break;
			Sleep(1000);
 			Out_List(computer_list, &computer_card);
 			printf("Computer show %d.\n", computer_card);
 			if (!computer_list->next)
				break;
 			printf("It's your turns.\n");
			user_card = User_against(user_list, computer_card);
			if(!user_card)
				goto loop;
 		}
 	}
 	if(!user_list->next)
 		printf("You win!\n");
 	else if(!computer_list->next){
 		printf("You lose!\n");
	}
 	else 
 		printf("System error!\n");
}

 int Computer_against(LinkList computer_list, int card){
	Sleep(1000);
	LinkList temp = computer_list->next;
	int computer_card;
	int judge;
	judge= 0;
	
	while(temp){
		if(temp->card > card){
			computer_card = temp->card;
			printf("Computer show %d.\n\n", temp->card);
 			Del_Link(computer_list, temp->card);
 			judge = 1;
 			break;
		}
		temp = temp->next;
	}
 	if(judge)
 		return computer_card;
	
 	else{
 		printf("Card's number is too large to against.\n\n");
 		return 0;
 	}
 }

int User_against(LinkList user_list, int card){
 	LinkList temp = user_list->next;
 	int user_card;
 	while(temp->next)
 		temp = temp->next;
 	if(temp->card <= card){
 		printf("You have no card to against opponent.\n\n");
 		return 0;
 	}
 	printf("Your cards are: \n");
 	Display_cards(user_list);
 	printf("Show your card now:\n");
	scanf("%d", &user_card);
 	while(user_card <= card || !Exist(user_list, user_card)){
 		getchar();
 		if(user_card <= card)
 			printf("Opponent show %d, please show a card larger than it.\n", card);
 		if(!Exist(user_list, user_card))
 			printf("Please show a card which you have.\n");
 		scanf("%d", &user_card);
	}
	
 	Del_Link(user_list, user_card);
 	return user_card;
}

int Show_card(){
	int card;
	int judge;
	while(1){
		judge=scanf("%d", &card);
		printf("%d\n",card);
		if( judge==1&&card > 1 && card < 11)
			break;
		printf("Please input a number of one of your cards.\n");
		getchar();
	}
	return card;
}

void Welcome(BSTree user_t, BSTree computer_t, LinkList user_l, LinkList computer_l){
	char chioce;
	while(1){
		system("cls");
		printf("-----------------------------------------------------------\n");
		printf("|	     ,            _..._            ,              |\n");
		printf("|	    {'.         .'     '.         .'}             |\n"); 
		printf("|	   { ~ '.      _|=    __|_      .'  ~}            |\n");
		printf("|	  { ~  ~ '-._ (___________) _.-'~  ~  }           |\n");
		printf("|	 {~  ~  ~   ~.'           '. ~    ~    }          |\n");
		printf("|	{  ~   ~  ~ /   /\\     /\\   \\   ~    ~  }         |\n");
		printf("|	{   ~   ~  /    __     __    \\ ~   ~    }         |\n");
		printf("|	 {   ~  /\\/  -<( o)   ( o)>-  \\/\\ ~   ~}          |\n");
		printf("|	  { ~   ;(      \\/ .-. \\/      );   ~ }           |\n");
		printf("|	   { ~ ~\\_  ()  ^ (   ) ^  ()  _/ ~  }            |\n");
		printf("|	    '-._~ \\   (`-._'-'_.-')   / ~_.-'             |\n");
		printf("|		'--\\   `'._'__'_.'`  /--'                 |\n");
		printf("|	            \\      \\`-'/    /                     |\n");
		printf("|	            `\\     '-'     /'                     |\n");
		printf("|	             `\\           /'                      |\n");
		printf("|	              `\\         /'                       |\n");
		printf("|	                ''-...-''                         |\n");
		printf("-----------------------------------------------------------\n");
		printf("Input '1' to play porker game.\n");
		scanf("%c", &chioce);
		getchar();
		if(chioce != '1')
			break;
		printf("Your cards are:\n");
		Init_List(&user_l);
	    Init_List(&computer_l);
		Create(&user_t, &computer_t);
		Tree_to_List(user_t, user_l);
		Tree_to_List(computer_t, computer_l);
	//	Display_cards(computer_l);
		Game_begins(user_l, computer_l);
		Clear(user_t, computer_t, user_l, computer_l);
		printf("Game over.\n");
		printf("You can input '1' to play this game again.\n");
		fflush(stdin);
		scanf("%c", &chioce);
		getchar();
		if(chioce != '1')
			break;
		}
}

void Clear_tree(BSTree root){
	if(root){
		Clear_tree(root->lchild);
		Clear_tree(root->rchild);
		free(root);
	}
}

void Clear_list(LinkList L){
	LinkList temp;
	while(L->next){
		temp = L->next;
		L->next = temp->next;
		free(temp);
	}
}

void Clear(BSTree user_t, BSTree computer_t, LinkList user_l, LinkList computer_l){
	Clear_tree(user_t);
	Clear_tree(computer_t);
	Clear_list(user_l);
	free(user_l);
	Clear_list(computer_l);
	free(computer_l);
}



int main(){
	BSTree user_tree, computer_tree;
	int x;
	LinkList user_list, computer_list;
	Welcome(user_tree, computer_tree, user_list, computer_list);
	return 0;
}

