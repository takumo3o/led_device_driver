#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/delay.h>

#define TENKAN 300

MODULE_AUTHOR("Takumi Ikeda");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.1");

static dev_t dev;
static struct cdev cdv;
static struct class *cls = NULL;
static volatile u32 *gpio_base = NULL;

void tanten(int sleep_count){
    int i;
    gpio_base[7] = 1 << 25;
    msleep(TENKAN);
    gpio_base[10] = 1 << 25;
    for(i = 0; i < sleep_count; i++){
        msleep(TENKAN);
    }
}

void tyoten(int sleep_count){
    int i;
    gpio_base[7] = 1 << 25;
    for(i = 0; i < 3; i++){
        msleep(TENKAN);    //msleepの値が1000を超えたくない
    }
    gpio_base[10] = 1 << 25;
    for(i = 0; i < sleep_count; i++){
        msleep(TENKAN);
    }
}
void dakuten(int sleep_count){
    tanten(1);
    tanten(sleep_count);
}

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
    char c;
    
    if(copy_from_user(&c,buf,sizeof(char)))
            return -EFAULT;

    if(c == '0'){
        //------------「チ」
        tanten(1);
        tanten(1);
        tyoten(1);
        tanten(3); 
        //------------「ク」
        tanten(1);
        tanten(1);
        tanten(1);
        tyoten(3);
        //------------「ワ」
        tyoten(1);
        tanten(1);
        tyoten(7);
    }
    else if(c == '1'){
        //------------「タ」
        tyoten(1);
        tanten(3);
        //------------「”」
        dakuten(3);
        //------------「イ」
        tanten(1);        
        tyoten(3);
        //------------「コ」
        tyoten(1);
        tyoten(1);
        tyoten(1);
        tyoten(3);
        //------------「ン」
        tanten(1);
        tyoten(1);
        tanten(1);
        tyoten(1);
        tanten(7);
    }
    else if(c == '2'){
        //------------「キ」
        tyoten(1);
        tanten(1);
        tyoten(1);
        tanten(1);
        tanten(3);
        //------------「”」
        dakuten(3);
        //------------「ユ」
        tyoten(1);
        tanten(1);
        tanten(1);
        tyoten(1);
        tyoten(3);
        //------------「ウ」
        tanten(1);
        tanten(1);
        tyoten(3);
        //------------「ス」
        tyoten(1);
        tyoten(1);
        tyoten(1);
        tanten(1);
        tyoten(3);
        //------------「シ」
        tyoten(1);
        tyoten(1);
        tanten(1);
        tyoten(1);
        tanten(3);
        //------------「”」
        dakuten(7);
    }
    return 1;
}

static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
    int size = 0;
    char sushi[] = {0xF0,0x9F,0x8D,0xA3,0x0A};//寿司の絵文字のバイナリ

    if (copy_to_user(buf+size,(const char *)sushi, sizeof(sushi))){
        printk( KERN_INFO "sushi : copy_to_user failed\n" );
        return -EFAULT;
    }
    size += sizeof(sushi);
    return size;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
    .read = sushi_read
};

static int __init init_mod(void)
{
    int retval;
      
    gpio_base = ioremap_nocache(0x3f200000, 0xA0);

    const u32 led = 25;
    const u32 index = led/10;
    const u32 shift = (led%10)*3;
    const u32 mask = ~(0x7 << shift);
    gpio_base[index] = (gpio_base[index] & mask) | (0x1 << shift);


    retval = alloc_chrdev_region(&dev, 0, 1, "myled");
    if(retval < 0){
            printk(KERN_ERR "alloc_chrdev_region failed.\n");
            return retval;
    }
    printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
                                              
    cdev_init(&cdv, &led_fops);
    cdv.owner = THIS_MODULE;
    retval = cdev_add(&cdv, dev, 1);
    if(retval < 0){
            printk(KERN_ERR "cdev_add failed. major:%d, minor%d", MAJOR(dev),MINOR(dev));
            return retval;
    }

    cls = class_create(THIS_MODULE,"myled");                                 if(IS_ERR(cls)){
            printk(KERN_ERR "class_create failed.");
                                                                                     return PTR_ERR(cls);                                             }
    device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));
  
    return 0;
}

static void __exit cleanup_mod(void)
{
      cdev_del(&cdv);
      device_destroy(cls, dev);
      class_destroy(cls);
      unregister_chrdev_region(dev, 1);
      printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
      iounmap(gpio_base);
}

module_init(init_mod);
module_exit(cleanup_mod);
