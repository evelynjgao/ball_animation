#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include <cstdlib>
#define PI 3.14159265    //gives PI value
using namespace std;

const int ball_off[]={15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 11, 10, 9, 8, 6, 3};  //how many pixels per quarter ball-row

class BallMove
{
public:

    BallMove();
    void change_color(void);
    void ShowBall(unsigned char *buf, int py);
    void CalculateBallPosition(void);
    void write_bmp_header_file(ofstream& output_file, int px, int pz);
    void Set_PX_Value(int value);
    void Set_PZ_Value(int value);
    void Set_Ball_X_Value(int value);
    void Set_Ball_Y_Value(int value);
    void Set_Ball_Speed(int value);
    int Get_PX_Value(void) const;
    int Get_PZ_Value(void) const;

private:
    int px;
    int pz;
    float delt_x, delt_y;
    int angle;
    int speed;
    float ball_x, ball_y;
    int ball_R, ball_G, ball_B;

};

void BallMove::change_color(void)           //changes ball color every bounce
{
    int a = rand();                    //generates a random number for the seed for extra randomness
    srand(a);

    ball_R = rand()%256 + 1;
    ball_B = rand()%256 + 1;
    ball_G = rand()%256 + 1;
}

void BallMove::ShowBall(unsigned char *buf, int py)       //shows ball size and shape
{
int j, dot;
int bx, by;

    by = ball_y;
    bx = ball_x;

    //cout << bx <<":"<<by <<endl;
    if(py<by-15 || py>by+15) return; //horizontal lines do not contain the ball

    //cout << py <<":"<<by <<endl;
    if(py >= by - 15 && py <= by)
    {
        dot = ball_off[by-py]*2 + 1;  //bottom half of ball
    }
    else
    {
        dot = ball_off[py-by]*2 + 1;    //top half of ball
    }

    for(j=0; j<dot; j++)           //ball color- every dot per ball
    {
        buf[3*(bx-dot/2+j)+0] = ball_R;
        buf[3*(bx-dot/2+j)+1] = ball_G;
        buf[3*(bx-dot/2+j)+2] = ball_B;
    }
}

void BallMove::CalculateBallPosition(void)      //calculates ball position relative to last
{
    ball_x += delt_x;                 //moves ball left/right for next image
    ball_y += delt_y;                 //moves ball up/down for next image

    if( ball_x >= px - 22 || ball_x < 22 ||       //if ball makes contact with left/right wall
       ball_y >= pz - 22 || ball_y < 22)           //if ball makes contact with top/bottom wall
    {
        if(ball_x >= px - 22)         //right wall
        {
            if(angle < 90)            //ball heading northeast
            {
                angle = 180 - angle;
            }
            else                      //ball heading southeast
            {
                angle = 540 - angle;
            }
        }

        if(ball_x < 22)                    //left wall
        {
            if(angle > 180)                //ball heading southwest
            {
                angle = 540 - angle;
            }
            else                         //ball heading northwest
            {
                angle = 180 - angle;
            }
        }
        if (ball_y >= pz - 22)     //top wall
        {
            if(angle > 90)                //ball heading northeast
            {
                angle = 360 - angle;
            }
            else                      //ball heading northwest
            {
                angle = 360 - angle;
            }
        }

        if(ball_y < 22)       //bottom wall
        {
            if(angle > 270)     //ball heading southeast
            {
                angle = 360 - angle;
            }
            else
            {
                angle = 360 - angle;
            }
        }


        ball_x -= delt_x;
        ball_y -= delt_y;


        delt_x = speed*cos(angle*PI/180);
        delt_y = speed*sin(angle*PI/180);

        ball_x += delt_x;
        ball_y += delt_y;
        change_color();
    }

    //ball_x += delt_x;
    //ball_y += delt_y;
}

void BallMove::write_bmp_header_file(ofstream& output_file, int px, int pz)
{
	unsigned short int bfType;
	bfType = 0x4D42;
    output_file.write ((char*)&bfType, sizeof (short int));

    unsigned long int bfSize;
	int rem;
	rem=3*px%4;
	int padding;
	if(rem==0)
	{
		padding=0;
	}
	else
	{
		padding=4-rem;
	}

	bfSize = 14 + 40 + (3 * px+padding) * pz;
//	bfSize = 14 + 40 + (3 * px+padding) * pz + 2;
    output_file.write ((char*)&bfSize, sizeof (long int));

    unsigned short int bfReserved1;
	bfReserved1 = 0;
    output_file.write ((char*)&bfReserved1, sizeof (short int));

	unsigned short int bfReserved2;
	bfReserved2 = 0;
    output_file .write ((char*)&bfReserved2, sizeof (short int));

	unsigned long int bfOffsetBits;
	bfOffsetBits = 14 + 40;
    output_file.write ((char*)&bfOffsetBits, sizeof (long int));

	unsigned long int biSize;
	biSize=40;
    output_file.write ((char*)&biSize, sizeof (long int));

	long int biWidth;
	biWidth=px;
    output_file.write ((char*)&biWidth, sizeof (long int));

	long int biHeight;
	biHeight=pz;
    output_file.write ((char*)&biHeight, sizeof (long int));

	unsigned short int biPlanes;
	biPlanes=1;
    output_file.write ((char*)&biPlanes, sizeof (short int));

	unsigned short int biBitCount;
	biBitCount=24;
    output_file.write ((char*)&biBitCount, sizeof (short int));

	unsigned long int biCompression;
// #define BI_RGB 0 f99fd43102
    unsigned long int bi_rgb = 0;
//	biCompression=BI_RGB;
	biCompression=bi_rgb;
    output_file.write ((char*)&biCompression, sizeof (long int));

	unsigned long int biSizeImage;
	biSizeImage=0;
    output_file.write ((char*)&biSizeImage, sizeof (long int));

	unsigned long int biXPelsPerMeter;
	biXPelsPerMeter=0;
    output_file.write ((char*)&biXPelsPerMeter, sizeof (long int));

	unsigned long int biYPelsPerMeter;
	biYPelsPerMeter=0;
    output_file.write ((char*)&biYPelsPerMeter, sizeof (long int));

	unsigned long int biClrUsed;
	biClrUsed = 0;
    output_file.write ((char*)&biClrUsed, sizeof (long int));

	unsigned long int biClrImportant;
	biClrImportant = 0;
    output_file.write ((char*)&biClrImportant, sizeof (long int));
}

void BallMove::Set_PX_Value(int value)
{
    px = value;
}

void BallMove::Set_PZ_Value(int value)
{
    pz = value;
}

void BallMove::Set_Ball_X_Value(int value)
{
    ball_x = value;
}

void BallMove::Set_Ball_Y_Value(int value)
{
    ball_y = value;
}

void BallMove::Set_Ball_Speed(int value)
{
    speed = value;
}

int BallMove::Get_PX_Value(void) const
{
    return px;
}

int BallMove::Get_PZ_Value(void) const
{
    return pz;
}

BallMove::BallMove()
{
    delt_x=4;
    delt_y=4;
    px = 512;       //width
    pz = 512;       //height
    ball_x = 60;    //ball max width
    ball_y = 60;    //ball max height
    angle = 30;     //initial starting
    speed = 12;      //
    delt_x = speed*cos(angle*PI/180);  //change in x
    delt_y = speed*sin(angle*PI/180);  //change in y
    ball_R = 255;   //initial starting color
    ball_G = 255;
    ball_B = 255;
}

int main(int argc, char * argv[])
{
    BallMove ball;

    ofstream ostrm;
    ifstream in_file;

    int number = 0;

	int i, j, bx, by, width, heigh;
    int my_r, my_g, my_b;
	unsigned char *buffer;
    string filename;

    if(argc == 2)                      //"config_data.txt" entered in command prompt line
    {
        filename = argv[1];

        cout << filename << endl;
        string temp;
        int value, variable_count;

        in_file.open(filename.c_str());   //opens "confic_data.txxt"
        if(in_file.fail())
        {
            cout << "Error opening config file : " << filename << endl;
        }
        in_file >> variable_count;       //first line- number that shows number of lines of data

        for(i=0; i<variable_count; i++)
        {
            in_file >> temp >> value;         //initialized string and value

            if(temp =="px")
            {
                ball.Set_PX_Value(value);
                //cout << "PX=" << value;
            }
            else if(temp =="pz")
            {
                ball.Set_PZ_Value(value);
                //cout << "PZ=" << value;
            }
            if(temp =="ball_x")
            {
                ball.Set_Ball_X_Value(value);
                //cout << "Ball_X=" << value;
            }
            if(temp =="ball_y")
            {
                ball.Set_Ball_Y_Value(value);
                //cout << "Ball_Y=" << value;
            }
            if(temp =="speed")
            {
                ball.Set_Ball_Speed(value);
                //cout << "speed=" << value;
            }
        }
        in_file.close();  //closes configuration file
    }

	width = ball.Get_PX_Value();          //gets image width
	heigh = ball.Get_PZ_Value();          //gets image height

    buffer = new unsigned char [width*3];   //total bites for all pixels in a row of the image


    for(number=0; number<1700; number++)   //number of ball bmp files
    {
        ostringstream output_file;

        output_file << "ball" << setfill('0') << setw(4) << number << ".bmp";  //names all the files
        //cout << output_file.str() << " "; //endl;

        filename = output_file.str();      c

        ostrm.open(filename.c_str(), ios::out | ios::binary);                 //open the file that was just created
        if(ostrm.fail())
        {
            cout << "Error.  Can't open output file " << output_file.str() <<"." << endl;
            return 0;
        }
        //cout << "Opening output file " << filename.c_str() << "." << endl;


        ball.write_bmp_header_file(ostrm, width, heigh);

        for(i=0;i<heigh;i++)
        {
            for(j=0; j<width; j++)             /
            {
                if(j<6 || j>=width-6)            //sets frame on top and bottom as white
                {
                    buffer[3*j+0] = 255;
                    buffer[3*j+1] = 255;
                    buffer[3*j+2] = 255;
                }
                else if(i<6 || i>width-6)      //sets frame on left and right as white
                {
                    buffer[3*j+0] = 255;
                    buffer[3*j+1] = 255;
                    buffer[3*j+2] = 255;
                }
                else                               //everything in the middle is black
                {
                    buffer[3*j+0] = 0;
                    buffer[3*j+1] = 0;
                    buffer[3*j+2] = 0;
                }
            }
            ball.ShowBall(buffer, i);
            ostrm.write ((char*)buffer, width*3*sizeof (unsigned char));
        }
        ostrm.close();
        ball.CalculateBallPosition();
     }


    delete [] buffer;
    return 0;
}
