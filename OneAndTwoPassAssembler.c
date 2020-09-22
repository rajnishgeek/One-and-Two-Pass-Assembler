#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>

//This is our one pass function
void onePassAssembler()
{
	FILE *read, *write;
	int current_address, starting_address;
	char opcode[20], label[20], operand[20];

	read = fopen("InputFile.txt", "r");
	write = fopen("SymbolTableFile.txt", "w");

	fscanf(read, "%s %s %s", label, opcode, operand);
	if (strcmp(opcode, "START") == 0)
	{
		starting_address = atoi(operand);
		current_address = starting_address;
	}

	while (!feof(read))
	{
		fscanf(read, "%s %s %s", label, opcode, &operand);
		if (strcmp(opcode, "HLT") == 0)
			printf("\n Program length = %d", current_address - starting_address);

		if (strcmp(label, "-") != 0)
		{
			fprintf(write, " %d \t %s \n", current_address, label);
			printf(" %d \t %s \n", current_address, label);
		}

		if (strcmp(opcode, "MOV") == 0 || strcmp(opcode, "SUB") == 0 || strcmp(opcode, "ADD") == 0 || strcmp(opcode, "INR") == 0 || strcmp(opcode, "INX") == 0
		        || strcmp(opcode, "DCX") == 0 || strcmp(opcode, "CMP") == 0 )
			current_address += 1;

		else if (strcmp(opcode, "JC") == 0 || strcmp(opcode, "JNZ") == 0 || strcmp(opcode, "JZ") == 0 || strcmp(opcode, "JNC") == 0)
			current_address += 3;

		else
			current_address += 2;
	}

	fclose(read);
	fclose(write);
}

//This is two pass assembler function
void twoPassAssembler()
{
	FILE *input_read, *output, *symbol_table_read;

	input_read = fopen("InputFile.txt", "r");
	int current_address, starting_address;
	char opcode[20], label[20], operand[20];

	fscanf(input_read, "%s %s %s", label, opcode, operand);

	if (strcmp(opcode, "START") == 0)
	{
		starting_address = atoi(operand);
		current_address = starting_address;
	}

	while (!feof(input_read))
	{
		fscanf(input_read, "%s %s %s", label, opcode, operand);

		if (strcmp(opcode, "HLT") == 0)
		{
			printf("%d \t 76 \n ", current_address);
			printf("\n Program Completed Execution");
			break;
		}

		output = fopen("OutputTableFile.txt", "r");
		symbol_table_read = fopen("SymbolTableFile.txt", "r");

		bool flag = false;
		char machine_code[20], temp_label[20];
		char temp_address[20];
		while (!feof(symbol_table_read))
		{

			fscanf(symbol_table_read, "%s %s", temp_address, temp_label);
			if (strcmp(operand, temp_label) == 0 )
			{
				flag = true;
				break;
			}
		}

		int label_address;
		if (flag == true)
		{
			label_address = atoi(temp_address);
//			printf("%d \n",label_address);
		}

		fclose(symbol_table_read);

		while (!feof(output))
		{
			char temp_opcode[20], temp_machine_code[20], temp_operand[20];
			fscanf(output, "%s %s %s", temp_opcode, temp_operand, temp_machine_code);
			if (strcmp(opcode, temp_opcode) == 0 && (flag == true || strcmp(operand, temp_operand) == 0) )
			{
				strcpy(machine_code, temp_machine_code);
				break;
			}
		}
		printf("%d \t %s ", current_address, machine_code);
		if (flag == true)
		{
			printf("%d \n", label_address);
			current_address += 3;
		}
		else
		{
			printf("\n");
			current_address += 1;

		}
		fclose(output);
	}

	fclose(input_read);
	getch();
}

//This is our main funciton
int main()
{
	printf("**********One Pass Assembler output****************\n");
	onePassAssembler();
	printf("\n\n**********************************************************\n\n");
	printf("**********Two Pass Assembler output****************\n");
	twoPassAssembler();
	return 0;
}
