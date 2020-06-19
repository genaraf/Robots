#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>		
#include <fcntl.h>		
#include <sys/ioctl.h>		

#include "interpret.h"
#include "icode.h"

#define I2CENABLE

#define ROBOT_I2C_ADDR 0x8

#define ROBOT_RC_DUR_LSB_CMD_REG    0x40
#define ROBOT_RC_DUR_MSB_CMD_REG    0x41
#define ROBOT_RC_CMD_REG    0x42

#define ROBOT_CMD_REG       0x43
#define ROBOT_CMD_MOUTH     0x44
#define ROBOT_CMD_SOUND     0x45

#define ROBOT_CMD_FWD   1
#define ROBOT_CMD_BACK  2
#define ROBOT_CMD_LEFT  3
#define ROBOT_CMD_RIGHT 4
#define ROBOT_CMD_DIMA  5
#define ROBOT_CMD_STOP  6

#ifdef I2CENABLE 
#include <linux/i2c-dev.h>	
 static int i2cfd = -1;
 static bool robotConnect() {
    if(i2cfd < 0) {
        if ((i2cfd = open("/dev/i2c-1", O_RDWR)) < 0)
       	{
            printf("Failed to open the i2c bus");
            return false;
        }
        int addr = ROBOT_I2C_ADDR;          // The I2C address of the slave
        if(ioctl(i2cfd, I2C_SLAVE, addr) < 0)
        {
            printf("Can't set the I2C address for the slave device.\n");
            return false;
    	}
    }
    return true;
 }
 
 static void robotCommand(uint8_t port, uint8_t command) {
    if(!robotConnect()) {
        return;
    }    
        
    uint8_t buffer[4];
    buffer[0] = port;
    buffer[1] = command;
    int length = 2;	// Number of bytes to write
    if(write(i2cfd, buffer, length) != length)
    {
    	printf("Failed to write to the i2c bus.\n");
    } 										
}

static void robotRCCommand(uint8_t command, uint16_t duration) {
    if(!robotConnect()) {
        return;
    }    
    uint8_t buffer[4];
    buffer[0] = ROBOT_RC_DUR_LSB_CMD_REG;
    buffer[1] = duration & 0xFF;
    if(write(i2cfd, buffer, 2) != 2)   {
    	printf("RC LSB Failed to write to the i2c bus.\n");
    } 										
    usleep(5000);
    buffer[0] = ROBOT_RC_DUR_MSB_CMD_REG;
    buffer[1] = (duration >> 8) & 0xFF;
    if(write(i2cfd, buffer, 2) != 2)   {
    	printf("RC MSB Failed to write to the i2c bus.\n");
    } 										
    usleep(5000);
    buffer[0] = ROBOT_RC_CMD_REG;
    buffer[1] = command;
    if(write(i2cfd, buffer, 2) != 2)   {
    	printf("RC MSB Failed to write to the i2c bus.\n");
    } 										
}
#else
 static void robotCommand(uint8_t port, uint8_t command) {};
 static void robotRCCommand(uint8_t command, uint16_t duration) {};
#endif

void ICodeInterpretator::initialize(const IProgram* icode) {
    assert(icode != 0);
    program = icode;
    currentCommand = icode->begin();
    stack.initialize();
}

void ICodeInterpretator::execute() 
{
    while (currentCommand != program->end()) {
        executeCommand();
    }
}

void ICodeInterpretator::executeCommand()
{
    assert(currentCommand != program->end());
    int commandType = currentCommand->type;
    NameDef* name;

    switch (commandType) {

        case ICommand::assign: {
            Value value0, value1;
            stack.pop(value1);
            stack.pop(value0);
            if (
                (value0.value_type & 
                (VALUE_TYPE_VARIABLE | VALUE_TYPE_ARRAY)) == 0
            ) {
                throw InterpretatorException(
                    "Left side of assigment is not an l-value"
                );
            }
            name = value0.name_ptr;
            assert(name != 0);
            Value res;
            if (name->baseType == TYPE_INT) {
                value1.convertToInt(res);
                if (value0.value_type == VALUE_TYPE_VARIABLE) {
                    name->value = res;
                } else {
                    assert(value0.value_type == VALUE_TYPE_ARRAY);
                    int idx = value0.int_value;
                    if (idx < 0 || idx >= name->dimension)
                        throw OutOfRangeException("Array index out of bounds");
                    ((int*) name->array)[idx] = res.int_value;
                }
            } else if (name->baseType == TYPE_DOUBLE) {
                value1.convertToDouble(res);
                if (value0.value_type == VALUE_TYPE_VARIABLE) {
                    name->value = res;
                } else {
                    assert(value0.value_type == VALUE_TYPE_ARRAY);
                    int idx = value0.int_value;
                    if (idx < 0 || idx >= name->dimension)
                        throw OutOfRangeException("Array index out of bounds");
                    ((double*) name->array)[idx] = res.double_value;
                }
            } else if (name->baseType == TYPE_STRING) {
                value1.convertToString(res);
                if (value0.value_type == VALUE_TYPE_VARIABLE) {
                    name->value = res;
                } else {
                    assert(value0.value_type == VALUE_TYPE_ARRAY);
                    int idx = value0.int_value;
                    if (idx < 0 || idx >= name->dimension)
                        throw OutOfRangeException("Array index out of bounds");
                    ((string*) name->array)[idx] = res.string_value;
                }
            }
            break;
        }

        case ICommand::int_const: {
            Value value;
            value.value_type = VALUE_TYPE_INT;
            value.int_value = currentCommand->int_value;
            stack.push(value);
            break;
        }

        case ICommand::double_const: {
            Value value;
            value.value_type = VALUE_TYPE_DOUBLE;
            value.double_value = currentCommand->double_value;
            stack.push(value);
            break;
        }

        case ICommand::string_const: {
            Value value;
            value.value_type = VALUE_TYPE_STRING;
            value.string_value = currentCommand->string_value;
            stack.push(value);
            break;
        }

        case ICommand::name: {  // Value of name
            if (currentCommand->name_ptr->value.value_type == 0) {
                char line[128];
                sprintf(
                    line,
                    "Value of variable %s was not defined",
                    currentCommand->name_ptr->name.c_str()
                );
                throw InterpretatorException(line);
            }
            stack.push(currentCommand->name_ptr->value);
            break;
        }

        case ICommand::name_lvalue: {  // L-Value of name
            name = currentCommand->name_ptr;
            Value value;
            value.name_ptr = name;
            value.value_type = VALUE_TYPE_VARIABLE;
            if (name->modifiers == NameDef::NAME_ARRAY)
                value.value_type = VALUE_TYPE_ARRAY;
            stack.push(value);
            break;
        }

        case ICommand::def_variable: {
            // Nothing to do
            break;
        }

        case ICommand::def_array: {
            name = currentCommand->name_ptr;
            Value dim, s;
            stack.pop(dim);
            dim.convertToInt(s);
            int n = s.int_value;
            name->allocateArray(n);
            break;
        }

        case ICommand::array_element: {
            Value a, idx;
            stack.pop(idx);
            stack.pop(a);
            if (a.value_type != VALUE_TYPE_ARRAY)
                throw InterpretatorException("Illegal array expression");
            Value index;
            idx.convertToInt(index);
            int n = index.int_value;
            if (n < 0 || n >= a.name_ptr->dimension)
                throw OutOfRangeException("Array index out of bounds");
            void* array = a.name_ptr->array;
            Value res;
            if (a.name_ptr->baseType == TYPE_INT) {
                res.value_type = VALUE_TYPE_INT;
                res.int_value = ((int*) array)[n];
            } else if (a.name_ptr->baseType == TYPE_DOUBLE) {
                res.value_type = VALUE_TYPE_DOUBLE;
                res.double_value = ((double*) array)[n];
            } else if (a.name_ptr->baseType == TYPE_STRING) {
                res.value_type = VALUE_TYPE_STRING;
                res.string_value = ((string*) array)[n];
            }
            stack.push(res);
            break;
        }

        case ICommand::array_lvalue: {
            Value a, idx;
            stack.pop(idx);
            stack.pop(a);
            if (a.value_type != VALUE_TYPE_ARRAY)
                throw InterpretatorException("Illegal array expression");
            Value index;
            idx.convertToInt(index);
            int n = index.int_value;
            if (n < 0 || n >= a.name_ptr->dimension)
                throw OutOfRangeException("Array index out of bounds");
            Value res;
            res.value_type = VALUE_TYPE_ARRAY;
            res.name_ptr = a.name_ptr;
            res.int_value = n;
            stack.push(res);
            break;
        }

        case ICommand::plus: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::add(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::minus: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::sub(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::mul: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::mul(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::div: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::div(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::mod: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::mod(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::uminus: {
            Value v0, res;
            stack.pop(v0);
            Value::negate(v0, res);
            stack.push(res);
            break;
        }

        case ICommand::cmp: {
            Value v0, v1, res;
            stack.pop(v1);
            stack.pop(v0);
            Value::compare(v0, v1, res);
            stack.push(res);
            break;
        }

        case ICommand::output: {
            Value v;
            stack.pop(v);
            v.print();
            break;
        }
        case ICommand::rc: {
            Value v;
            stack.pop(v);
            Value v2;
            stack.pop(v2);
            if(v.int_value < 0) v.int_value = 500;
            if(v.int_value > 20000) v.int_value = 20000;
            printf("RC command: %d, %d ms\r\n", v2.int_value, v.int_value);
            robotRCCommand(v2.int_value, v.int_value);
            usleep((v.int_value + 50) * 1000);
            break;
        }
        case ICommand::right: {
            printf("right command\r\n"); 
            robotCommand(ROBOT_CMD_REG, ROBOT_CMD_RIGHT);
            break;
        }
        case ICommand::left: {
            printf("left command\r\n"); 
            robotCommand(ROBOT_CMD_REG, ROBOT_CMD_LEFT);
            break;
        }
        case ICommand::back: {
            printf("back command\r\n"); 
            robotCommand(ROBOT_CMD_REG, ROBOT_CMD_BACK);            
            break;
        }
        case ICommand::forward: {
            printf("forward command\r\n"); 
       	    robotCommand(ROBOT_CMD_REG, ROBOT_CMD_FWD);
            break;
        }
        case ICommand::dima: {
            printf("dima command\r\n"); 
            robotCommand(ROBOT_CMD_REG, ROBOT_CMD_DIMA);
            break;
        }
        case ICommand::sleep: {
            int time = 3000;
            printf("sleep command: ");
            Value v;
            stack.pop(v);
            v.print();
            printf(" ms\r\n");
            if(v.isInt() && v.int_value > 0) 
            {
                time = v.int_value;
                usleep(time*1000);
            }
            break;
        }
        case ICommand::stop: {
            printf("stop command\r\n");
            robotCommand(ROBOT_CMD_REG, ROBOT_CMD_STOP);
            break;
        }
        case ICommand::mouth: {
            printf("mouth command:");
            Value v;
            stack.pop(v);
            v.print();
            printf("\r\n");
            robotCommand(ROBOT_CMD_MOUTH, v.int_value);
            break;
        }
         case ICommand::sound: {
            printf("sound command:");
            Value v;
            stack.pop(v);
            v.print();
            printf("\r\n");
            robotCommand(ROBOT_CMD_SOUND, v.int_value);
            break;
        }
        case ICommand::input: {
            Value v;
            stack.pop(v);
            v.scanf();
            break;
        }

        case ICommand::go_to: {
            const Label* label = currentCommand->goto_label_ptr;
            assert(label->number != 0);
            //... if (label->location == 0)
            //...     throw InterpretatorException("Undefined label");
            currentCommand = label->location;
            return;
        }

        case ICommand::if_goto: {
            Value v;
            stack.pop(v);
            assert(v.value_type == VALUE_TYPE_INT);
            int res = v.int_value;
            int cond = currentCommand->ext;
            if (
                (cond == ICommand::eq && res == 0) ||
                (cond == ICommand::ne && res != 0) ||
                (cond == ICommand::gt && res > 0) ||
                (cond == ICommand::lt && res < 0) ||
                (cond == ICommand::ge && res >= 0) ||
                (cond == ICommand::le && res <= 0)
            ) {
                const Label* label = currentCommand->goto_label_ptr;
                assert(label->number != 0);
                //... if (label->location == 0)
                //...     throw InterpretatorException("Undefined label");
                currentCommand = label->location;
                return;
            }
            break;
        }
    }
    ++currentCommand;
}

void ICodeInterpretator::printStackTop() const {
    if (stack.size() <= 0) {
        printf("Stack empty"); 
        return;
    }
    stack.top().print();
}
