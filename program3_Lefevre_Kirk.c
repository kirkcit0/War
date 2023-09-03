//Kirk Lefevre
//Dr. Steinberg
//COP3502 Spring 2022
//Programming Assignment 3

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct card_s{
	int rank;    //number
	char * type; //type of card
	struct card_s * nextptr;
}card_t;

//function prototypes
void rules(); //display rules
int playRound(); //simulate round
card_t * openCardDeck(); //open the card deck and place into a linkedlist setup
card_t * insertBackSetup(card_t *node, char *name, int cardrank); //take card from orginial deck and place in back of linked list for setup of game
int empty(card_t * node); //check to see if linked list is empty
void cleanUp(card_t * head); //free memory to prevent memory leaks
int deckSize(card_t * head); //count number of nodes in the linked list
card_t * search(card_t * node, int spot); //search list for a specific spot in the card deck indexing is similar to array setup
card_t * copyCard(card_t * node); //make a deep copy of card
card_t * removeCard(card_t * node, int spot); //remove card from linkedlist
card_t * insertBackDeck(card_t *head, card_t *node); //place card at end of pile
int compareCard(card_t * card, card_t * cardp2); //compare cards
card_t * moveCardBack(card_t *head); //place card at top of deck to the bottom of the deck

int main()
{
	int seed;
	printf("Enter Seed: ");
	scanf("%d", &seed);
	
	srand(seed); //seed set
	rules();
	
	int player; //1 or 2
	int result;
	
	printf("Would you like to be player 1 or 2?\n");
	printf("Enter 1 or 2: ");
	scanf("%d", &player); //choose player
	
	for(int game = 1; game <= 5; ++game) //simulate games
	{
		printf("Alright lets play game %d.\n", game);
		printf("Lets split the deck.\n");

		result = playRound(); //play game
		
		if((result == 1 && player == 1) || (result == 2 && player == 2)) //determine who won
			printf("You won game %d.\n", game);
		else
			printf("I won game %d.\n", game);
	}
	
	return 0;
}

void rules()
{
	printf("Welcome to the card game war!\n");
	printf("Here are the rules.\n");
	printf("You have a pile of cards and I have a pile of cards.\n");
	printf("We will each pull the top card off of our respective deck and compare them.\n");
	printf("The card with the highest number will win the round and take both cards.\n");
	printf("However if there is a tie, then we have to we have to place one card faced down and the next one faced up and compare the results.\n");
	printf("Winner of the tie, will grab all the cards out. However if it's a tie again, then we repeat the same action.\n");
	printf("Ready? Here we go!\n");
}

card_t * openCardDeck()
{
	FILE *fptr = fopen("deck.txt", "r");
	
	char *name = (char *) malloc(sizeof(char) * 20);
	
	if (name == NULL)
	{
		printf("Error in malloc...\n");
		exit(1);
	}
	
	card_t * head = NULL;
	
	int cardrank = 13;
	int tracker = 1;
	while(fscanf(fptr, "%s", name) == 1)
	{			
		strcat(name, " ");
		
		if(tracker % 5 == 1)
		{
			strcat(name, "of Clubs");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 2)
		{
			strcat(name, "of Diamonds");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 3)
		{
			strcat(name, "of Hearts");
			head = insertBackSetup(head, name, cardrank);
		}
		else if(tracker % 5 == 4)
		{
			strcat(name, "of Spades");
			head = insertBackSetup(head, name, cardrank);
			tracker = 0;
			--cardrank;
		}
		
		++tracker;
		
	}
	
	free(name);
	fclose(fptr);
	
	return head;
}

card_t * insertBackSetup(card_t *node, char *name, int cardrank)
{
	
    if(empty(node)) //check to see if list is empty
    {
		node = (card_t *) malloc(sizeof(card_t));
		
		if(empty(node)) //check to see if malloc worked
		{
			printf("Did not allocate head successfully...");
			printf("Program Terminating...\n");
			exit(1);
		}
		else //otherwise populate data of new head node
		{
			node->type = (char *) malloc(sizeof(char) * 20);
			
			if(node->type == NULL)
			{
				printf("Error with malloc...\n");
				exit(1);
			}
			
			strcpy(node->type, name);
			node->rank = cardrank;
			node->nextptr = NULL; //must make new tail nextptr NULL!!!
			
			return node;  //terminate
		}
      
    }
	
	//here we know that the list has at least one node
	
	card_t *head = node; //keep pointer to head since we will need to return this address
	
	while(node->nextptr != NULL) //traverse to tail
		node = node->nextptr;
	
	node->nextptr = (card_t *) malloc(sizeof(card_t)); 
	
	if(node->nextptr == NULL) //see if new tail was allocated successfully
	{
		printf("Did not allocate node successfully...");
		return head; //terminate if tail didn't get created
	}
	
	//populate new node
	node->nextptr->type = (char *) malloc(sizeof(char) * 20);
	
	if(node->nextptr->type == NULL)
	{
		printf("Error with malloc...\n");
		exit(1);
	}
	
	strcpy(node->nextptr->type, name);
	node->nextptr->rank = cardrank;
	node->nextptr->nextptr = NULL; //very important
			
	return head; //return head node since we need our starting point of the linked list
}

int empty(card_t * node)
{
	return (node == NULL); //return condition result
}

int playRound(){

	int x=0;
//declare a potentially unneeded amount of struct variables. mallocate some of them.
	card_t * card=(card_t*)malloc(sizeof(card_t));
	card_t * cardp1=(card_t*)malloc(sizeof(card_t));
	card_t * cardp2=(card_t*)malloc(sizeof(card_t));
	card_t * temp;
	card_t * temp1;
	card_t * temp2;
	card_t * temp3;
	if(card==NULL || card==NULL || cardp2==NULL){
		printf("oops malloc didnt feel like mallocing. Sorry :(");
		exit(1);
	}
//keep mallocating <3
	card->type=(char *) malloc(sizeof(char) * 20);
	cardp1->type=(char *) malloc(sizeof(char) * 20);
	cardp2->type=(char *) malloc(sizeof(char) * 20);
	if(card->type==NULL || cardp1->type==NULL || cardp2->type==NULL){
		printf("malloc didnt work lol");
		exit(1);
	}
//open the deck, count how many cards.
	card=openCardDeck();
	int cardnum=deckSize(card);

	printf("There are %d cards in the deck.\n", cardnum);

	int spot;
	//split that deck
	while(1){
		//random variable, temp is the card thats drawn randomly, and temp3 is used because using only temp caused segfaults.
		spot=rand()%cardnum;
		temp=search(card, spot);
		temp3=copyCard(temp);
		//insertbackdeck doesnt work for the head, so we use copycard if its the first card.
		if(x<2){
			cardp1=copyCard(temp3);
			x++;
		}
		//or we use insertbackdeck otherwise
		else
			cardp1=insertBackDeck(cardp1, temp3);

		//temp1 represents the first player's deck. remove the card we added to the deck.
		temp1=search(card, spot);
		temp1=temp1->nextptr;
		card=removeCard(card, spot);
		//there is now one less card.
		cardnum--;
		//break the loop if no more cards remain
		if(cardnum==0){
			break;
		}
		//if we remove the head, go to the next card.
		if(card==NULL){
			if(card->nextptr!=NULL)
				card=card->nextptr;
		}


		//same as player one's drawing.
		spot=rand()%cardnum;
		temp=search(card, spot);
		temp3=copyCard(temp);

		if(x<2){
			cardp2=copyCard(temp3);
			x++;
		}
		else
			cardp2=insertBackDeck(cardp2, temp3);

		temp2=search(card, spot);
		temp2=temp2->nextptr;
		card=removeCard(card, spot);
		
		cardnum--;

		if(cardnum==0){
			break;
		}
		if(card==NULL){
			if(card->nextptr!=NULL)
				card=card->nextptr;
		}
	}
	cleanUp(card);


	int p1size=deckSize(cardp1);
	int p2size=deckSize(cardp2);
	spot=0;
	int warnum;
	int y;

	while(1){

		printf("Player 1 pulled out a %s.\tPlayer 2 pulled out a %s.\n", cardp1->type, cardp2->type);
		//set the temps to the heads
		temp1=cardp1;
		temp2=cardp2;
		//compare the cards
		int comp=compareCard(cardp1, cardp2);
		if(comp==1){
			printf("Player 1 won that round.\n");
			//move the card they drew to the back, insert player 2's card, remove the card inserted from player2.
			cardp1=moveCardBack(cardp1);
			cardp1=insertBackDeck(cardp1, cardp2);
			cardp2=removeCard(cardp2, spot);
		}
		//same as before, but player 2 won.
		else if(comp==2){
			printf("Player 2 won that round.\n");
			cardp2=moveCardBack(cardp2);
			cardp2=insertBackDeck(cardp2, cardp1);
			cardp1=removeCard(cardp1, spot);
		}
		//WAR!?!?!??!?
		else if(comp==0){
			//warnum is the amount of cards drawn in total. y means the current war ended.
			warnum=6;
			y=0;
			while(1){
				//if y isnt 0, it means a new war within the war so increase warnum.
				if(y!=0){
					warnum+=warnum;
				}
				y++;
				printf("Ugh oh...We have a tie! W-A-R!\n");
				if(temp1->nextptr == NULL || temp1->nextptr == NULL){
					break;
				}
				//move to the next 2 cards, one facedown one faceup.
				temp1=temp1->nextptr->nextptr;
				temp2=temp2->nextptr->nextptr;
				
				x=0;
				printf("Player 1 pulled out a %s.\tPlayer 2 pulled out a %s.\n", temp1->type, temp2->type);
				comp=compareCard(temp1, temp2);
				//if the last card in the deck is the same as the card drawn by the opponent, the opponent with more cards wins.
				if(deckSize(cardp2)==1 && comp==0){
					return 1;
				}
				else if(deckSize(cardp1)==1 && comp==0){
					return 2;
				}
				//player 1 wins, use the same logic as earlier.
				if(comp==1){
					printf("Player 1 won that round.\n");
					cardp1=moveCardBack(cardp1);
					while(x<warnum){
						//if player2 runs out of cards, break the loop
						if(cardp2==NULL)
							break;
						cardp1=insertBackDeck(cardp1, cardp2);
						cardp2=removeCard(cardp2, spot);
						x++;
					}
					break;
				}
				//same as player 1 winning
				else if(comp==2){
					printf("Player 2 won that round.\n");
					cardp2=moveCardBack(cardp2);
					while(x<warnum){
						if(cardp1==NULL)
							break;
						cardp2=insertBackDeck(cardp2, cardp1);
						cardp1=removeCard(cardp1, spot);
						x++;
					}
					break;
				}
			}
		}
	
		//count how many cards are in each player's deck
		p1size=deckSize(cardp1);
		p2size=deckSize(cardp2);

		printf("Player 1 has %d cards.\n", p1size);
		printf("Player 2 has %d cards.\n", p2size);
		//if someone has no cards, the winner is returned.
		if(p1size==0){
			cleanUp(cardp1);
			cleanUp(cardp2);
			return 2;
		}
		else if(p2size==0){
			cleanUp(cardp2);
			cleanUp(cardp1);
			return 1;
		}
	}
	return 0;

}

void cleanUp(card_t * head){
	while(head!=NULL){	
		card_t * goodnight=head;
		head=head->nextptr;
		free(goodnight);
	}
}

int deckSize(card_t * head){
	int x=0;
	//mallocation time
	card_t * temp=malloc(sizeof(head));
	temp->type=(char*)malloc(sizeof(head->type));
	temp->nextptr=(card_t*)malloc(sizeof(head));

	temp=head;
	//count that decksize
	while(temp!=NULL){
		x++;
		temp=temp->nextptr;
	}
	//free temp, its not necessary
	cleanUp(temp);

	return x;
}

card_t * search(card_t * node, int spot){
	int idx=0;
	//if spot is 0, it's the head
	if(idx==spot){
		return node;
	}
	//otherwise count up until we hit that spot and return it
	while(idx!=spot){
		node=node->nextptr;
		idx++;
	}
	return node;
}

card_t * copyCard(card_t * node){
	//mallocate memory!
	card_t * ncopy=(card_t*)malloc(sizeof(node));
	if(ncopy==NULL){
		printf("damn1\n");
		exit(1);
	}
	ncopy->type = (char*)malloc(sizeof(node->type));
	if(ncopy==NULL){
		printf("damn2\n");
		exit(1);
	}
	//deep copy <3
	ncopy->rank=node->rank;
	ncopy->type=node->type;
	ncopy->nextptr=NULL;

	return ncopy;
}

card_t * removeCard(card_t * node, int spot){
	card_t * head=node;
	card_t * temp=search(node, spot);
	//if we're removing the head, move the head over 1 and free the temp that's equal to the original head
	if(head==temp){
		head=head->nextptr;
		free(temp);
		return head;
	}
	//traverse to temp
	while(node->nextptr!=temp){
		node=node->nextptr;
	}
	//if it's not the last card, move node to point to the card after temp. so we can remove it.
	if(temp->nextptr!=NULL){
		node->nextptr=temp->nextptr;
		free(temp);
		return head;
	}
	//if it is the last card, snip off temp lol
	node->nextptr=NULL;
	free(temp);
	return head;
}

card_t * insertBackDeck(card_t *head, card_t *node){
	//if there's no head, just make node the head
	if(head==NULL){
		head=node;
		return head;
	}

	card_t * temp=head;
	card_t * nodehead=copyCard(node);
	//if there is a head, traverse to the end
	while(temp->nextptr!=NULL){
		temp=temp->nextptr;
	}
	//and set the next pointer to the node youre adding.
	temp->nextptr=nodehead;

	return head;
}

card_t * moveCardBack(card_t *head){
	card_t * temp=head;
	//traverse to the end
	while(temp->nextptr!=NULL){
		temp=temp->nextptr;
	}
	//last card points to head
	temp->nextptr=head;
	//head moves to next ptr (new head)
	head=head->nextptr;
	//nullify what's pointing to head
	temp->nextptr->nextptr=NULL;
	
	return head;
}

int compareCard(card_t * card, card_t * cardp2){
	//if player 1 has a stronger card return 1
	if(card->rank>cardp2->rank)
		return 1;
	//if not, return 2
	else if(cardp2->rank>card->rank)
		return 2;
	//if equal, WAR! return 0
	else
		return 0;
}








