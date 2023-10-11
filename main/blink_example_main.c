#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "time.h"

//7SEG PINS

#define aPin        15
#define bPin        2
#define cPin        4
#define dPin        5
#define ePin        18
#define fPin        19
#define gPin        21

// TRANSISTOR PINS

#define a1ControlPin    23
#define a2ControlPin    13
#define a3ControlPin    12
#define a4ControlPin    14

// BUTTONS

#define btn1Pin     25
#define btn2Pin     22
#define btn3Pin     33

// COLON

#define ledColon    27

// time_t seconds;
// struct tm *timeStruct;

// seconds = time(NULL);

// timeStruct = localtime(&seconds);

static const char *TAG = "example";

uint8_t  dstmin = 0;
uint8_t  min = 0;
uint8_t  dstsec = 0;
uint8_t sec = 0;


int pins[7] = {dPin,cPin,ePin,gPin,fPin,aPin,bPin};
int hex[10][7] =    {{1,1,1,0,1,1,1}, //0
                     {0,1,0,0,0,0,1}, //1
                     {1,0,1,1,0,1,1}, //2
                     {1,1,0,1,0,1,1}, //3
                     {0,1,0,1,1,0,1}, //4
                     {1,1,0,1,1,1,0}, //5
                     {1,1,1,1,1,1,0}, //6
                     {0,1,0,0,0,1,1}, //7
                     {1,1,1,1,1,1,1}, //8
                     {1,1,0,1,1,1,1}, //9
                     };

void board_config(){
    gpio_reset_pin(cPin);
    gpio_reset_pin(dPin);
    gpio_reset_pin(ePin);
    gpio_reset_pin(bPin);
    gpio_reset_pin(aPin);
    gpio_reset_pin(fPin);
    gpio_reset_pin(gPin);
    
    gpio_reset_pin(btn1Pin);
    gpio_reset_pin(btn2Pin);
    gpio_reset_pin(btn3Pin);

    gpio_reset_pin(ledColon);

    gpio_reset_pin(a1ControlPin);
    gpio_reset_pin(a2ControlPin);
    gpio_reset_pin(a3ControlPin);
    gpio_reset_pin(a4ControlPin);
    
    gpio_set_direction(cPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(dPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(ePin, GPIO_MODE_OUTPUT);
    gpio_set_direction(bPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(aPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(fPin, GPIO_MODE_OUTPUT);
    gpio_set_direction(gPin, GPIO_MODE_OUTPUT);

    gpio_set_direction(btn1Pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(btn1Pin, GPIO_PULLUP_ONLY);
    gpio_set_direction(btn2Pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(btn2Pin, GPIO_PULLUP_ONLY);
    gpio_set_direction(btn3Pin, GPIO_MODE_INPUT);
    gpio_set_pull_mode(btn3Pin, GPIO_PULLUP_ONLY);

    gpio_set_direction(ledColon, GPIO_MODE_INPUT_OUTPUT);

    gpio_set_direction(a1ControlPin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(a2ControlPin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(a3ControlPin, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(a4ControlPin, GPIO_MODE_INPUT_OUTPUT);
}

bool edit_pressed()
{
    return (gpio_get_level(btn1Pin) == 0);
}

bool add()
{
    return (gpio_get_level(btn2Pin) == 0);
}

bool confirm_pressed()
{
    return (gpio_get_level(btn3Pin) == 0);
}

void counter(){
    if(sec==10){
        sec=0;
        dstsec++;
    }
    
    if (dstsec==6){
        dstsec=0;
        min++;
    }
    if (dstsec==9 && sec==9 ){
       dstsec=0;
       sec=0;
    }
    

    if(min == 10){
        min = 0;
        dstmin++;
    }

    if (dstmin==9 && min==9 ){
       dstmin=0;
       min=0;
    }

}

void seg_reset(){
    gpio_set_level(a1ControlPin,0);
    gpio_set_level(a2ControlPin,0);
    gpio_set_level(a3ControlPin,0);
    gpio_set_level(a4ControlPin,0);
}

void shining(){

        gpio_set_level(ledColon,1);
        ESP_LOGI(TAG,"svieti");
}


void value_reset(){
    dstmin=0;
    min=0;
    dstsec=0;
    sec=0;
}



void app_main(void){
    board_config();
    while (1){
        ESP_LOGW(TAG,"sekunda");

        for(int j = 0; j < 14401; j++){
            gpio_set_level(a1ControlPin,0);
            gpio_set_level(a2ControlPin,0);
            gpio_set_level(a3ControlPin,0);
            gpio_set_level(a4ControlPin,1);
            if (gpio_get_level(a4ControlPin)){
                for(int i = 0; i < 7; i++){
                    gpio_set_level(pins[i], hex[sec][i]);
                    }
            }
            vTaskDelay(8 / portTICK_PERIOD_MS);
            gpio_set_level(a4ControlPin,0);
            gpio_set_level(a3ControlPin,1);
            if (gpio_get_level(a3ControlPin)){
                for(int i = 0; i < 7; i++){
                    gpio_set_level(pins[i], hex[dstsec][i]);
                    }
            }
            vTaskDelay(8 / portTICK_PERIOD_MS);
            gpio_set_level(ledColon,0);
            gpio_set_level(a3ControlPin,0);
            gpio_set_level(a2ControlPin,1);
            if (gpio_get_level(a2ControlPin)){
                for(int i = 0; i < 7; i++){
                    gpio_set_level(pins[i], hex[min][i]);
                }
            }
            vTaskDelay(8 / portTICK_PERIOD_MS);
            gpio_set_level(ledColon,0);
            gpio_set_level(a2ControlPin,0);
            gpio_set_level(a1ControlPin,1);
            if (gpio_get_level(a1ControlPin)){
                for(int i = 0; i < 7; i++){
                    gpio_set_level(pins[i], hex[dstmin][i]);

                }
            }

            if(edit_pressed()){
    
                    bool edit = 1;
                    while (edit_pressed()){
                    vTaskDelay(1); 
                    }
                                        
                    if(edit){

                        ESP_LOGW(TAG,"edit");
                        
                        seg_reset();
                        int x = 0;               

                        while(x == 0){

                            seg_reset();
                            gpio_set_level(a1ControlPin,1);

                            if(edit_pressed()){
                                    x++;
                                    ESP_LOGW(TAG,"zmena");
                                    gpio_set_level(a2ControlPin,0);
                                    gpio_set_level(a2ControlPin,1);
                                    for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[min][i]);
                                 }
                                while (edit_pressed()){
                                        vTaskDelay(1); 
                                    }

                                
                            }
                            if(add()){                                
                                ESP_LOGW(TAG,"add");
                                dstmin++;
                                if (dstmin>9){
                                    dstmin=0;
                                }
                                for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[dstmin][i]);

                                }
                                while (add()){
                                        vTaskDelay(1); 
                                    }

                                }


                        
                            if(confirm_pressed()){
                                ESP_LOGW(TAG,"confirm");
                                while (confirm_pressed()){
                                    vTaskDelay(1); 
                                }
                                break;
                            
                            }
                        }

                        while(x == 1){
                            seg_reset();

                            gpio_set_level(a2ControlPin,1);

                            if(edit_pressed()){
                                    x++;
                                    gpio_set_level(a3ControlPin,0);
                                    gpio_set_level(a3ControlPin,1);
                                    for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[dstsec][i]);
                                    ESP_LOGW(TAG,"zmena");
                                    }
                                while (edit_pressed()){
                                        vTaskDelay(1); 
                                    }
                                    
                                }
                        
                            if(add()){
                                
                                ESP_LOGW(TAG,"add");
                                min++;
                                if (min>9){
                                    min=0;
                                }                                
                                for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[min][i]);

                                }
                                while (add()){
                                        vTaskDelay(1); 
                                    }
                                
                            }
                            if(confirm_pressed()){
                                ESP_LOGW(TAG,"confirm");
                                while (confirm_pressed()){
                                    vTaskDelay(1); 
                                }
                                break;
                            }
                        }

                        while(x == 2){

                            seg_reset();

                            gpio_set_level(a3ControlPin,1);

                            if(edit_pressed()){
                                    x++;
                                    ESP_LOGW(TAG,"zmena");
                                    gpio_set_level(a4ControlPin,0);
                                    gpio_set_level(a4ControlPin,1);
                                    for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[sec][i]);
                                    ESP_LOGW(TAG,"zmena");
                                    }                                 
                                while (edit_pressed()){
                                        vTaskDelay(1); 
                                    }
                                    
                                }
                        
                            if(add()){
                                
                                ESP_LOGW(TAG,"add");
                                dstsec++;
                                if (dstsec>9){
                                    dstsec=0;
                                }                                
                                for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[dstsec][i]);

                                }
                                while (add()){
                                        vTaskDelay(1); 
                                    }

                            }
                            if(confirm_pressed()){
                                ESP_LOGW(TAG,"confirm");
                                while (confirm_pressed()){
                                    vTaskDelay(1); 
                                }
                                break;
                            }
                        }

                        while(x == 3){

                            seg_reset();

                            gpio_set_level(a4ControlPin,1);

                            if(edit_pressed()){
                                    x = 0;
                                    ESP_LOGW(TAG,"zmena");
                                    gpio_set_level(a1ControlPin,0);
                                    gpio_set_level(a1ControlPin,1);
                                    for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[dstmin][i]);
                                    }
                                while (edit_pressed()){
                                        vTaskDelay(1); 
                                    }
                                    
                                }
                        
                            if(add()){
                                
                                ESP_LOGW(TAG,"add");
                                sec++;
                                if (sec>9){
                                    sec=0;
                                }                                
                                for(int i = 0; i < 7; i++){
                                    gpio_set_level(pins[i], hex[sec][i]);

                                }
                                while (add()){
                                        vTaskDelay(1); 
                                    }
                                
                            }
                            if(confirm_pressed()){
                                ESP_LOGW(TAG,"confirm");
                                while (confirm_pressed()){
                                    vTaskDelay(1); 
                                }
                                break;
                            }
                        }

                    }
                }

            vTaskDelay(8 / portTICK_PERIOD_MS);
            if(j == 14400){
                seg_reset();
                ESP_LOGW(TAG,"sekunda");
                shining();
                sec++;
                counter();
                j = 0;
            }

        }

    }

}