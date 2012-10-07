#include<stdio.h>
#include<string.h>
#include<gtk/gtk.h>
int repeater();
//statstics_variable_declaration.
int charger_status;
int charge;
struct time
{
	int hour;
	int minute;
	int second;
}remaining_time;
//Gtk_widget_declaration
GtkWidget *window;
GtkWidget *bg;
GtkWidget *layout;
GtkWidget *vbox;
GtkWidget *label[2];
GtkWidget *close_event;
GtkWidget *close_img;
GtkWidget *power;
//Gtk_widget initialization
void gtk_initialize()
{
	int i;
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window,200,75);
	gtk_window_set_opacity(GTK_WINDOW(window),.5);
	//gtk_window_set_modal(GTK_WINDOW(window),FALSE);	
	//gtk_window_set_keep_below(GTK_WINDOW(window),TRUE);
	gtk_window_set_decorated(GTK_WINDOW(window),FALSE);
	gtk_window_set_resizable(GTK_WINDOW(window),FALSE);
	gtk_window_set_skip_taskbar_hint(GTK_WINDOW(window),TRUE);
	gtk_window_stick(GTK_WINDOW(window));
	bg=gtk_image_new_from_file("./images/danger.png");
	power=gtk_image_new();
	gtk_window_move(GTK_WINDOW(window),gdk_screen_width()-200,gdk_screen_height()-200);
	vbox=gtk_vbox_new(0,0);
	layout=gtk_layout_new(NULL,NULL);
	for(i=0;i<2;i++)
	{
		label[i]=gtk_label_new("Initializing..");
	}
	close_event=gtk_event_box_new();
	close_img=gtk_image_new_from_file("./images/close.png");
}
void gtk_signals()
{
	g_signal_connect(close_event,"button_press_event",gtk_main_quit,NULL);
}
void gtk_packing()
{
	int i;
	gtk_container_add(GTK_CONTAINER(window),layout);
	gtk_layout_put(GTK_LAYOUT(layout),bg,0,0);
	gtk_layout_put(GTK_LAYOUT(layout),power,10,0);
	gtk_layout_put(GTK_LAYOUT(layout),vbox,40,20);
	
	gtk_layout_put(GTK_LAYOUT(layout),close_event,180,0);
	for(i=0;i<2;i++)
	{
		gtk_box_pack_start(GTK_BOX(vbox),label[i],1,1,0);
	}
	gtk_container_add(GTK_CONTAINER(close_event),close_img);
}
void destroy(GtkWidget *button,GtkWidget *notif)
{
	gtk_widget_destroy(notif);
}
void notify(int x)
{
	if(x==1)
	{
		gint result;
		GtkWidget* message=gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
GTK_MESSAGE_WARNING,
GTK_BUTTONS_OK,"Battery is full\nRemove Charger to avoid damage");
		gtk_widget_show_all(message);
		result=gtk_dialog_run(GTK_DIALOG(message));
		gtk_widget_destroy (message);
		repeater();
		if(charger_status==1)
		{
			notify(1);
		}
		

	}
	else
	{
		gint result;
		GtkWidget* message=gtk_message_dialog_new (NULL, GTK_DIALOG_MODAL,
GTK_MESSAGE_WARNING,
GTK_BUTTONS_OK,"Battery is goint to be empty\nPlease replace it or plug in charger");
		gtk_widget_show_all(message);
		result=gtk_dialog_run(GTK_DIALOG(message));
		gtk_widget_destroy (message);
		repeater();
		if(charger_status==0)
		{
			notify(1);
		}
	}
		
}	
	
void display_gtk()
{
	char text[100];
	if(charger_status==0)
	{
		gtk_image_set_from_file(GTK_IMAGE(power),NULL);
	}
	else
	{
		
		gtk_image_set_from_file(GTK_IMAGE(power),"./images/power.png");
	}
 

    	
    	
	sprintf(text,"<span size=\"xx-large\" ><b> %d%% </b></span>" ,charge);
	gtk_label_set_markup(GTK_LABEL(label[0]),text);
	sprintf(text,"%d hour %d minutes remaining",remaining_time.hour,remaining_time.minute);
	gtk_label_set_text(GTK_LABEL(label[1]),text);
	if(!gtk_window_is_active(GTK_WINDOW(window)))
	{
		gtk_window_deiconify(GTK_WINDOW(window));
		//gtk_window_set_keep_below(GTK_WINDOW(window),TRUE);
	}
	if(charge==100)
	{
		gtk_image_set_from_file(GTK_IMAGE(bg),"./images/full.png");
	}
	else if(charge>60)
	{
		gtk_image_set_from_file(GTK_IMAGE(bg),"./images/75.png");
	}
	else if(charge>35)
	{
		gtk_image_set_from_file(GTK_IMAGE(bg),"./images/50.png");
	}
	else if(charge>10)
	{
		gtk_image_set_from_file(GTK_IMAGE(bg),"./images/25.png");
	}
	else
	{
		gtk_image_set_from_file(GTK_IMAGE(bg),"./images/danger.png");
	}
	if(charger_status==1&&charge==100)
	{
		notify(1);
	}
	else if(charger_status==0&&charge<10)
	{
		notify(0);
	}
}
	
void decode(char status[][15])
{
	int i;
	if(strcmp(status[0],"off-line")==0)
	{
		charger_status=0;
	}
	else
	{
		charger_status=1;
	}
	i=0;
	charge=0;
	while(status[1][i]!='%')
	{
		charge=charge*10+(int)(status[1][i]-48);
		i++;
	}
	remaining_time.hour=(int)(status[2][0]-48)*10+(int)(status[2][1]-48);
	remaining_time.minute=(int)(status[2][3]-48)*10+(int)(status[2][4]-48);
	remaining_time.second=(int)(status[2][6]-48)*10+(int)(status[2][7]-48);
}
void display_text()
{
	if(charger_status==0)
	{
		printf("charger is offline\n");
	}
	else
	{
		printf("charger is online\n");
	}
	printf("Charge on battery is %d%%\n",charge);
	printf("Remaining time is %d hours %d minutes and %d seconds\n",remaining_time.hour,remaining_time.minute,remaining_time.second);
	
}
void read_status()
{
	FILE *fp;
	char ch;
	char status[4][15];
	int i=0,j=0;
	//execute system command and store result in file
	system("acpi -a|cut -d' ' -f3 > temp");
	system("acpi|cut -d' ' -f4 >> temp");
	system("acpi|cut -d' ' -f5 >> temp");
//	system("acpi|cut -d' ' -f6 > temp");
	//open file and read result
	fp=fopen("temp","r");
	while(fscanf(fp,"%c",&ch)!=EOF)
	{
		if(ch=='\n')
		{
			status[i][j]=0;
			i++;
			j=0;
		}
		else
		{
			status[i][j]=ch;
			j++;
		}
	}
	status[i][j]=0;
	fclose(fp);
	//printf("no issue with reading\n");
	decode(status);
	//printf("no issue with decoding\n");
	//return 0;
}
int repeater()
{
	/*static int i=15;
	i--;
	charge=i;
	charger_status=0;
	remaining_time.hour=1;
	remaining_time.minute=1;
	remaining_time.second=1;
	
	display_text();
	display_gtk();*/
	static int i=0;
	char text[10];
	//while(1)
	{
		int j;
		system("clear");
		//sleep(2);
		printf("\n");
		read_status();
		display_text();
		display_gtk();
		for(j=i;j>=0;j--)
		{
			printf("=");
		}
		i++;
		//sleep(2);
		gtk_main_iteration();
	}
//	sleep(2);
	return 1;
}
int main(int argc,char *argv[])
{
	gtk_init(&argc,&argv);
	gtk_initialize();
	gtk_signals();
	gtk_packing();
	//repeater();
	g_timeout_add_seconds(2,repeater,NULL);
	gtk_widget_show_all(window);
	gtk_main();
	//repeater();
	
	
}
		
	
	
