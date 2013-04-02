using System;
using Microsoft.SPOT;
using Microsoft.SPOT.Hardware;
using Microsoft.SPOT.Presentation;
using Microsoft.SPOT.Presentation.Media;

namespace Graphics1
{
    public class Program
    {
        public static void Main()
        {
			Debug.Print("This is my main\r\n");
			
            //Bitmap LCD = new Bitmap(SystemMetrics.ScreenWidth, SystemMetrics.ScreenHeight);
			/*Bitmap LCD = new Bitmap(150,150);
 
            //clears the memory and not the display
            //LCD.Clear();
            //draw on memory
            LCD.DrawLine(Colors.Red, 10, 0, 0, 149, 149);
			//LCD.SetPixel(10, 10, Colors.White);
            //transfer the bitmap memory to the actual display
            LCD.Flush();*/

			
			int x0, x1, y0, y1;
			int cc = 0;
			Bitmap LCD = new Bitmap(200, 200);
			
			x0 = 10;
			y0 = 10;
			x1 = 190;
			y1 = 190;
			while (true){
				LCD.Clear();
				LCD.DrawLine(Colors.Red, 3, x0, y0, x1, y1);
				if (cc == 0){
					x0 += 5;
					x1 -= 5;
				} else {
					x0 -= 5;
					x1 += 5;
				}
				if (x0 >= 190)
					cc = 1;
				else if (x0 <= 10)
					cc = 0;

				LCD.Flush();
			}

			/*Bitmap LCD = new Bitmap(143,157);            
			Bitmap image = new Bitmap("face.bmp");
            LCD.DrawImage(0,0,image,0,0,143,157,0);		
            LCD.Flush();    */   
		}
    }
}
