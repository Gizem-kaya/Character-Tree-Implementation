#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max 200


struct node{

	char letter;
	char* password;
	struct node* pointer_child;
	struct node* pointer_sibling;
};

typedef struct node node;


node* create_root(){			// for creating the first node of the character tree as the root.

	node *root_node = malloc(sizeof(node));

    if ( root_node ) {
        root_node->pointer_child = NULL;
        root_node->pointer_sibling = NULL;
        root_node->password = NULL;
        root_node->letter = '\0';
    }

    return root_node;			// returns the root's pointer.

}


node* new_node(char data){			// creates a basic and half-empty new node with character information.

    node *new_node = malloc(sizeof(node));

    new_node->pointer_child = NULL;
    new_node->pointer_sibling = NULL;
    new_node->letter = data;
	new_node->password = NULL;


    return new_node;		// returns the new node's pointer.
}

void add_password(node * current_node, char password[]){		// for adding the password information to coming node pointer that is points to the last character of the name.

		current_node->password = malloc(sizeof(password)+1);

        strcpy(current_node->password, password);

    	//printf("password of %c: %s\n", current_node->letter, current_node->password);


}


node* add_sibling(node * root, char data){			// creates a new node and adds the node to given node as sibling.


	
    while (root->pointer_sibling){			// goes to the last node because new node must be added to the end of the siblings.

        root = root->pointer_sibling;
    }
	//printf("%c's new sibling is %c\n",root->letter, data);
	root->pointer_sibling = new_node(data);

    return (root->pointer_sibling);				// returns the new node's pointer.
}

node* add_new_child(node * root, char data){  // creates and adds a new child node to given node.

		//printf("%c's new child is %c\n",root->letter, data);
    	root->pointer_child = new_node(data);
        return (root->pointer_child);            // returns the new node's pointer.
}


int add_name(node ** root, char name[], char password[], int pointer_counter, FILE*write_file_ptr){    // adds the given name to the character tree.


	int i = 0;
	int j = 0;
	int k;
	struct node* temp_node = *root;				// for defining a temporary node pointer.

	if(temp_node->pointer_child){				// if this is not the first name of the tree.

		temp_node = (*root)->pointer_child;


		i = 0 ;
		for (k = 0; name[k]; k++){	   // finds the name array's size.


			while(temp_node->letter != name[k] && temp_node->pointer_sibling){   // for finding the first same character of the tree. if there is no same character, then temp_node points the last character of the siblings of the first characters.

				temp_node = temp_node->pointer_sibling;

			}

			if(temp_node->letter == name[k] && temp_node->pointer_child && name[i+1]){  // if we found a character which is same as the first character of the name array.


				temp_node = temp_node->pointer_child;
				i++;

				if((!temp_node->pointer_child) && (temp_node->letter == name[i]) ){

					j = 1;
					i++;

				}

			}

		}
	}

	else{
			temp_node = *root;
		}

		if(!name[i] && !temp_node->pointer_child && temp_node->password){

			fprintf(write_file_ptr,"\"%s\" reserved username \n", name);
			return 1;
		}

		else if(!name[i]){

			add_password(temp_node, password);
			return 0;
		}


		for (; name[i]; i++,j++){



			if(pointer_counter == 0){  // for the first name of the program.

				temp_node = add_new_child(temp_node, name[i]);

			}
			else if(j == 0 && temp_node->letter == name[i] && !name[i+1]){
		
				add_password(temp_node, password);
				return 0;
			}
			else if(j == 0 && i != 0){			// first time iterating.


				temp_node = add_sibling(temp_node, name[i]);

			}
			else if(j == 0 && i == 0 ){			// first time iterating.

				temp_node = add_sibling(temp_node, name[i]);

			}

			else{			// other cases.
			

				temp_node = add_new_child(temp_node, name[i]);

			}
			if(!name[i+1]){  // if the coming letter is the final letter of the name.
		
				add_password(temp_node, password);
				return 0;

			}


		}


	return 0;
}

int send_query(node * root, char name[], char password[], FILE*write_file_ptr){

	struct node* temp_node = root;	// for defining a temporary node pointer.


	int k,l;

	temp_node = temp_node->pointer_child;

	for (l = 0; name[l]; l++){	}

	for (k = 0; name[k]; k++){


		while(temp_node && temp_node->letter != name[k])
			temp_node = temp_node->pointer_sibling;


		if(!temp_node && k == 0){
			fprintf(write_file_ptr,"\"%s\" no record\n",name);
			return 0;

		}
		else if(!temp_node){
			fprintf(write_file_ptr,"\"%s\" incorrect username\n",name);
			return 0;
		}

		if(k == l-1 && temp_node->password){

			if(!strcmp(temp_node->password, password)){
				fprintf(write_file_ptr,"\"%s\" successful login\n", name);
				return 0;
			}
			else if(strcmp(temp_node->password, password)){
				fprintf(write_file_ptr,"\"%s\" incorrect password\n",name);
				return 0;

			}

		}


		else if(!temp_node && k > 0){
			fprintf(write_file_ptr,"\"%s\"22 incorrect username\n",name);
			return 0;
		}


			temp_node = temp_node->pointer_child;

	}

	if(name[k]){
			if(temp_node->password != NULL){
				
				fprintf(write_file_ptr,"\"%s\" password \"%s\" \n",name,temp_node->password);
				return 0;
			}
			else{
				fprintf(write_file_ptr,"\"%s\" not enough username\n",name);
				return 0;
			}

		}
	else if(temp_node){

		fprintf(write_file_ptr,"\"%s\" not enough username\n",name);
		return 0;
	}

	else{
		fprintf(write_file_ptr,"\"%s\" no record\n",name);
		return 0;
	}

	return 0;

}


int search(node * root, char name[], FILE*write_file_ptr){


	struct node* temp_node = root;	// for defining a temporary node pointer.


	int k,l;

	temp_node = temp_node->pointer_child;

	for (l = 0; name[l]; l++){	}

	for (k = 0; name[k]; k++){


		if(k == l-1 && temp_node->password){

			fprintf(write_file_ptr,"\"%s\" password \"%s\" \n", name, temp_node->password);
			return 0;
		}

		while(temp_node != NULL && temp_node->letter != name[k]){

			temp_node = temp_node->pointer_sibling;

		}

		if(!temp_node && k == 0){
			fprintf(write_file_ptr,"\"%s\" no record\n",name);
			return 0;

		}
		else if(!temp_node && k > 0){
			fprintf(write_file_ptr,"\"%s\" incorrect username\n",name);
			return 0;
		}


			temp_node = temp_node->pointer_child;
			if(!temp_node){
				fprintf(write_file_ptr,"\"%s\" incorrect username\n",name);
				return 0;
			}
	}
	if(temp_node){
			if(temp_node->password != NULL && name[k] ){
			
				fprintf(write_file_ptr,"\"%s\" password \"%s\" \n",name,temp_node->password);
				return 0;
			}
			else{
				fprintf(write_file_ptr,"\"%s\" not enough username\n",name);
				return 0;

			}

		}
	else{
		fprintf(write_file_ptr,"\"%s\" no record\n",name);
		return 0;
	}

	return 0;
}



int delete_name( node* root, char name[], FILE*write_file_ptr){


	struct node* temp_node = root;	// for defining a temporary node pointer.



	int k,l;

	temp_node = temp_node->pointer_child;

	for (l = 0; name[l]; l++){	}

	for (k = 0; name[k]; k++){


		if(k == l-1 && temp_node->password){

			temp_node->password = NULL;
			fprintf(write_file_ptr,"\"%s\" deletion is successful\n", name);
			return 0;
		}

		while(temp_node != NULL && temp_node->letter != name[k]){

			temp_node = temp_node->pointer_sibling;

		}

		if(!temp_node && k == 0){
			fprintf(write_file_ptr,"\"%s\" no record\n",name);
			return 0;

		}
		else if(!temp_node && k > 0){
			fprintf(write_file_ptr,"\"%s\" incorrect username\n",name);
			return 0;
		}


			temp_node = temp_node->pointer_child;
	}
	if(temp_node){
			if(temp_node->password != NULL){
				temp_node->password = NULL;
				fprintf(write_file_ptr,"\"%s\" deletion is successful\n", name);
			}
			else{
				fprintf(write_file_ptr,"\"%s\" not enough username\n",name);

			}

		}
	else{
		fprintf(write_file_ptr,"\"%s\" no record\n",name);
	}

	return 0;


}



int print_tree(node* root,char ass[], FILE*write_file_ptr){

	struct node* temp_node = root;
	if (root == NULL){
        return 1;
	}

	
    int i= 0;
    for( i = 0 ;i < 25 ; i++){
        if(ass[i]== '2'){
            break;
        }
    }

    char arr[25];
    strcpy(arr,ass);
    if(root->letter){
         arr[i]=root->letter;

    }

    int temp,f;
    while (root)
    {
        if(root->pointer_child){
                if(root->pointer_child->pointer_sibling && arr[0]!='2'){
                    fprintf(write_file_ptr,"-");
                    for(f=0;f<25;f++){
                        if(arr[f]!='2'){
                            fprintf(write_file_ptr,"%c",arr[f]);
                        }
                    }fprintf(write_file_ptr,"\n");

                }

        }

        temp=print_tree(root->pointer_child,arr,write_file_ptr);


        if(temp==1 ){
        	
                fprintf(write_file_ptr,"\t");
            for(f=0;f<25;f++){
                if(arr[f]!='2'){
                            fprintf(write_file_ptr,"%c",arr[f]);
                }

            }fprintf(write_file_ptr,"\n");
        }
          
        
         else if(root->pointer_sibling){
            arr[i]='2';
            arr[i]=root->pointer_sibling->letter;
            }
        root = root->pointer_sibling;

    }
    return 0;
}



int main(int argc, char *argv[]) {



	FILE* input_file;
	input_file = fopen(argv[1],"r");
	
	FILE* close_file_ptr = fopen("output.txt", "w");       /* if the output file exists and contains something, deletes it.*/
	fclose(close_file_ptr);
	
	
	FILE* write_file_ptr = fopen("output.txt", "a");		/* opens the output file.*/
	
	
    node *root = create_root();

	char* temporary = (char*)malloc(max*sizeof(char));
	char* temporary2 = (char*)malloc(max*sizeof(char));

	int pointer_counter = 0;

	while(fscanf(input_file, "%s", temporary) == 1){

		if(!strcmp(temporary,"-a")){

			fscanf(input_file, "%s", temporary);

			fscanf(input_file, "%s", temporary2);

			if (add_name(&root, temporary, temporary2, pointer_counter, write_file_ptr) == 0){


				fprintf(write_file_ptr,"\"%s\" was added\n",temporary);
			}


			pointer_counter = 1;


		}
		else if(!strcmp(temporary,"-s")){

			fscanf(input_file, "%s", temporary);
			search(root, temporary, write_file_ptr);

		}
		else if(!strcmp(temporary,"-q")){


			fscanf(input_file, "%s", temporary);
			fscanf(input_file, "%s", temporary2);

			send_query(root, temporary, temporary2, write_file_ptr);


		}
		else if(!strcmp(temporary,"-d")){

			fscanf(input_file, "%s", temporary);
			delete_name(root, temporary, write_file_ptr);


		}

		else if(!strcmp(temporary,"-l")){


			char ass[25];
			int t;
			for(t=0;t<24;t++){
               ass[t]='2';
			}
			ass[24]='\0';
		
			print_tree(root, ass, write_file_ptr);
		

		}

	}

	fclose(write_file_ptr);
	fclose(input_file);
	return 0;
}
