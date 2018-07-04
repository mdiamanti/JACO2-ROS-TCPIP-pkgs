#include <ros/ros.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <std_msgs/String.h>

#define PORT 8080

using namespace std;


class CurStateListener
{
private:
    std_msgs::String curState;
    bool receivedState;
public:
    void setReceivedState()
    {
        receivedState = false;
    }

    std_msgs::String getCurState()
    {
	return curState;
    }
	
    bool getReceivedState()
    {
	return receivedState;
    }

    void stateCallBack(const std_msgs::String& msg)
    {
	curState = msg;
	receivedState = true;
    }
};



int main (int argc, char** argv)
{
    // Initialize ROS node
    ros::init(argc, argv, "server_node");
 
    // Create a handle to this process' node
    ros::NodeHandle nh;

    // Create publisher and subscriber objects
    ros::Publisher posPub = nh.advertise<std_msgs::String>("/joint_positions_from_plc", 100);
    CurStateListener stateListener;
    ros::Subscriber stateSub = nh.subscribe("/current_state_to_plc", 100, &CurStateListener::stateCallBack, &stateListener);

    // Establish TCP IP communication through sockets
    int sockfd, newsockfd, n ; // socket file descriptors and port number
    int enable = 1; 
    socklen_t clilen; 
    char buffer[256];
    char *buf; 
    struct sockaddr_in serv_addr, cli_addr; // objects to store client and server address

    cout << "Server node is listening on port " << PORT << " for incoming connections" << endl;

    // Create socket file descriptor
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("ERROR on opening socket");
        exit(EXIT_FAILURE);
    }

    // Prevent errors 
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("ERROR on setsockopt");
        exit(EXIT_FAILURE);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    // Attach socket to port 8080
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("ERROR on binding");
        exit(EXIT_FAILURE);
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0){
       perror("ERROR on accept");
       exit(EXIT_FAILURE);
    }

    // Declare variables
    std_msgs::String positions;
    stringstream ss;

    while(ros::ok()){	
        ss.str(std::string()); // clear stringstream and buffer
        ss.clear();
        bzero(buffer,256);

        // Read joint position message from socket 
        n = read(newsockfd,buffer,255);
        if (n < 0){
            perror("ERROR on reading from socket");
            exit(EXIT_FAILURE);
        }
      
        // Publish joint position message on "/joint_positions_from_plc" topic 
        ss << buffer;
        positions.data = ss.str();
        ROS_INFO("%s", positions.data.c_str());
        while (posPub.getNumSubscribers() == 0){
            ros::spinOnce();
        }
        ROS_INFO("Got subscriber");
        posPub.publish(positions);
        ROS_INFO("The following joint position message is just published on /joint_positions_from_plc topic\n%s", positions.data.c_str());

        // Subscribe to "/current_state_to_plc" topic, so as to read the current joint state
        stateListener.setReceivedState();
	      ROS_INFO("Waiting for current joint state to arrive from joint_trajectory_client node");
        while (!stateListener.getReceivedState()){
            ros::spinOnce();
        }
        ROS_INFO("Current joint state just arrived\n%s", stateListener.getCurState().data.c_str());
        buf = buffer;
        buf = strncpy(buffer, stateListener.getCurState().data.c_str(), sizeof buffer-1);
        ROS_INFO("%s",buffer);

        // Write current joint state message to socket
        n = write(newsockfd,buffer,strlen(buf));
        if (n < 0){
            perror("ERROR on writing to socket");
            exit(EXIT_FAILURE);
        }

        //close(newsockfd);
        //close(sockfd);	
    }
	
    return 0;
}
