#define LED1 27 // GPIO 12
#define LED2 26 // etc.
#define LED3 25

struct s_led {
    byte gpio; // LED GPIO number
    byte state; // LED state
    unsigned napms; // Delay to use (ms)
    TaskHandle_t taskh; // Task handle
};
static s_led leds[3] = {
    { LED1, 0, 500,  },
    { LED2, 0, 200, 0 },
    { LED3, 0, 750, 0 }
};

static void led_task_func(void *argp) {
    s_led *ledp = (s_led*)argp;
    unsigned stack_hwm = 0, temp;


    for (; ;) {
        digitalWrite(ledp->gpio,ledp->state ^= 1);
        temp = uxTaskGetStackHighWaterMark(nullptr);

        if ( !stack_hwm || temp < stack_hwm ) {
            stack_hwm = temp;
            printf("Task for gpio %d has stack hwm %u\n", ledp->gpio,stack_hwm);
        }
        delay(ledp->napms);
    }
}

void setup() {
    int app_cpu = 0; // CPU number

    delay(500); // Pause for serial setup

    app_cpu = xPortGetCoreID();
    printf("app_cpu is %d (%s core)\n",
        app_cpu,
        app_cpu > 0 ? "Dual" : "Single");

    printf("LEDs on gpios: ");
    for ( auto& led : leds ) {
        pinMode(led.gpio,OUTPUT);
        digitalWrite(led.gpio,LOW);
        xTaskCreatePinnedToCore(
            led_task_func,
            "led_task",
            2048,
            &led,
            1,
            &led.taskh,
            app_cpu
        );
        printf("%d ",led.gpio);
    }
    putchar('\n');
}

void loop() {
  delay(1000); // delay zodat er tijd tussen het opnieuw runnen zit

  if (count == 10) {
    prinf("Pauze for 3 seconden!");

    int i;
    for (i=0; i!=3; i++) {
      vTaskSuspend(leds[i].taskh); // pauzeer iedere led taak (totaal 3)
    }

    delay(3000); // 3 sec. delay

    for (i=0; i!=3; i++) { // opnieuw starten van de taken
      vTaskResume(leds[i].taskh);
    }

    count = 0;
  }
}
