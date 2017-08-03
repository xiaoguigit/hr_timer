#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/delay.h>



static struct  hrtimer hr_timer;
static ktime_t ktime;
static unsigned int interval = 10; /* unit: us */
static int cnt = 0;
static struct work_struct vibe_work;
struct timespec start_uptime;  


static void vibe_work_func(struct work_struct *work)  //DO-->工作队列函数
{ 

    //ktime = ktime_set( 0, (interval%1000000)*1000); 
    //hrtimer_start(&hr_timer,ktime,HRTIMER_MODE_REL);

   hrtimer_cancel(&hr_timer);
	
} 

long my_abs(long a, long b)
{
    if(a-b > 0){
        return a -b;
    }else{
        return b-a;
    }
}


enum hrtimer_restart my_hrtimer_callback( struct hrtimer *timer )
{  
    struct timespec uptime;  
    getrawmonotonic(&uptime);  
    //printk("Time:%lu.%06lu\n",(unsigned long) uptime.tv_sec,(uptime.tv_nsec));  
    //printk("Time:%lu.%06lu\n",(unsigned long) start_uptime.tv_sec,(start_uptime.tv_nsec ));  
    printk("use Time:%lu.%06lu\n",(unsigned long) uptime.tv_sec - start_uptime.tv_sec,my_abs(start_uptime.tv_nsec, uptime.tv_nsec)/1000);  
    schedule_work(&vibe_work);
    return HRTIMER_NORESTART;
}
  


void my_hr_timer_start(void)
{
    ktime = ktime_set( 0, (interval%1000000)*1000); 
    hr_timer.function = my_hrtimer_callback;
    hrtimer_start(&hr_timer,ktime,HRTIMER_MODE_REL);

}



   

static int __init hello_init(void)
{
        
    printk("<1>Hello Kernel!\n"); /*立即输出信息*/
    INIT_WORK(&vibe_work, vibe_work_func);  /* Intialize the work queue */  //初始化工作队列
    hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_hr_timer_start();
    getrawmonotonic(&start_uptime);  
    //printk("Time:%lu.%06lu\n",(unsigned long) uptime.tv_sec,(uptime.tv_nsec / (NSEC_PER_SEC / 100)));  
    return 0;
}

static void __exit hello_cleanup(void)
{
    printk("<1>See you,Kernel!\n");

    hrtimer_cancel(&hr_timer);
}

module_init(hello_init);        /*进入内核的时候，调用hello_init        */
module_exit(hello_cleanup);     /*离开内核的时候，调用hello_cleanup     */
MODULE_LICENSE("GPL");          /*协议*/

