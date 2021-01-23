#include<bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
using namespace std;

template<class ver = string>
class uGraph {
    using edgeList = list<pair<ver, float>>;
    map<ver, edgeList> adj;

    edgeList &neighbours(ver u) {
      return adj[u];
    }

    list<ver> &vertices() {
      static list<ver> vList;
      if (!vList.empty()) return vList;
      for (auto i = adj.begin(); i != adj.end(); ++i)
        vList.push_back(i->first);
      return vList;
    }

public:
    list<ver> path;

    void addEdge(ver u, ver v, float wt) {
      adj[u].push_back({v, wt});
      adj[v].push_back({u, wt});
    }

    void dijkstra_get_sssp(ver destination, map<ver, ver> &predecessor) {
      for (; !destination.empty(); destination = predecessor[destination]) {
        path.push_back(destination);
      }
      path.reverse();
      cout << "\t\t\t";
      copy(all(path), ostream_iterator<ver>(cout, "\n\t\t\t"));
    }

    bool print_adj(ver src, ver des) {
      int f = 0;
      //Let try to print the adj list
      //Iterate over all the key value pairs in the map
      for (auto &j : adj) {
        f += j.first == src;
        //Iterator over the list of cities
        for (auto &l : j.second)
          f += l.first == des;
      }
      return f > 1;
    }

    void dijkstra_sssp(ver src, map<ver, float> &d, map<ver, ver> &prev) {
      set<pair<float, ver> > PQ;
      prev = {};
      auto relax = [&](ver u, ver v, float wt) {
          if (d[u] + wt < d[v]) {
            //In the set updating of a particular is not possible
            // we have to remove the old pair, and insert the new pair to simulation updating
            if (auto f = PQ.find({d[v], v});
                    f != PQ.end())
              PQ.erase(f);

            //Insert the new pair
            d[v] = d[u] + wt;
            prev[v] = u;
            PQ.insert({d[v], v});
          }
      };
      for (auto &v : vertices()) {
        d[v] = numeric_limits<float>::infinity();//set d[v] = +INF
        prev[v] = "";//set prev[v] -> NIL
      }

      d[src] = 0;
      PQ.insert({d[src], src});

      while (!PQ.empty()) {

        //Find the pair at the front.
        auto[ignore, u] = *(PQ.begin());

        PQ.erase(PQ.begin());
        //Iterate over neighbours/children of the current u
        for (auto[v, wt]: neighbours(u)) {
          relax(u, v, wt);//relax all neighbouring vertices
        }
      }
    }

    void make_dot_file() {
      string a, b, clr;
      string labl;
      int f;
      f = 0;
      clr = "red";
      char filename[] = "finalmap.dot";
      string delimiter = ",";
      ofstream fout(filename);
      fout << "graph G {" << endl;
      fout << "node [shape=rect,dpi=600] margin=0.75" << endl;
      fout << "\n//" << clr << endl;
      string x;
      ifstream file("data.txt");
      while (!file.eof()) {
        getline(file, x);
        vector<string> v = split(x, ',');
        a = v[0];
        b = v[1];
        labl = v[2];
        if (f == 1) {
          fout << "\n//" << clr << endl;
          f = 0;
        }

        if (check(a, b) == true)
          fout << "\"" << a << "\"" << " -- " << "\"" << b << "\"" << " " << "[label=\"" << labl << "\",color=" << clr
               << " " << ",penwidth=\"8\"];" << endl;
        else
          fout << "\"" << a << "\"" << " -- " << "\"" << b << "\"" << " " << "[label=\"" << labl << "\",color=" << clr
               << " " << ",penwidth=\"2\"];" << endl;
        if (a == "Seelampur" && f == 0) {
          f = 1;
          clr = "blue";
        }
        else if (a == "Golf Course" && f == 0) {
          f = 1;
          clr = "green";
        }
        else if (a == "Sant Guru Ram Singh Marg" && f == 0) {
          f = 1;
          clr = "violet";
        }
        else if (a == "JL Nehru Stadium" && f == 0) {
          f = 1;
          clr = "yellow";
        }
      }
      fout << "}";
    }

    vector<string> split(const string &s, char delim) {
      vector<string> result;
      stringstream ss(s);
      string item;

      while (getline(ss, item, delim)) {
        result.push_back(item);
      }

      return result;
    }

    bool check(ver src, ver des) {//check if path is possible
      int f = 0;
      for (auto &v : path)
        f += v == src || v == des;
      return f == 2;
    }

    void calc_price(ver src, ver des) {
      /*fare calculation and prints fare using fare.csv*/
      string line, num;
      int row = 0, col = 0, k = 0, len = 0, last = 0;
      long arr[235][235];
      ifstream infile("fare.csv");//read fare.csv excel database

      if (infile.is_open()) {
        while (getline(infile, line)) {
          // Takes complete row
          // cout << line<< '\n';
          k = 0, last = 0, col = 0;
          len = line.length();
          while (k != len) {
            if (line[k] == ',' || k == len - 1) {
              //for converting string into number
              arr[row][col] = atof(num.append(line, last, k).c_str());
              //Emptying string for getting next data
              num.clear();

              //increasing column number after saving data
              col++;

              if (k != len - 1)
                last = k + 1;
            }
            k++;
          }
          //increase row number for array
          row++;
        }
        //close the file
        infile.close();
      }
      else cerr << "Unable to open file";
      string line1;
      int a = 0, b = 0, f = 0;
      ifstream file("stations.txt");

      while (!file.eof()) {
        getline(file, line1);
        auto v1 = split(line1, '\t');
        if (v1[1] == src) {
          f++;
          std::istringstream(v1[0]) >> a;
        }
        if (v1[1] == des) {
          f++;
          std::istringstream(v1[0]) >> b;
        }
        if (f == 2) //both src and des covered
          break;
      }
      cout << endl << "\t\t\t";
      cout << "--> Fare is: र" << arr[a - 1][b - 1] << endl;
    }
};

template<class ver = string>
class App {
    uGraph<> metro;

    void get_input(ver &src, ver &des) {
      cout << endl << endl << endl;
      cout << "\t\t";
      cout << "Enter source station : ";
      getline(cin, src);

      cout << endl;
      cout << "\t\t";
      cout << "Enter destination station : ";

      getline(cin, des);
    }

    void handle_error() {
      system("zenity --error --title \"Error Occured\" --text='Invalid Station Entered'");
      system("clear");
    }

    void display_welcome() {
      std::cout << R"(
888b     d888 8888888888 88888888888 8888888b.   .d88888b.            8888888888 Y88b   d88P  .d88888b.  8888888b.  888     888  .d8888b.
8888b   d8888 888            888     888   Y88b d88P" "Y88b           888         Y88b d88P  d88P" "Y88b 888  "Y88b 888     888 d88P  Y88b
88888b.d88888 888            888     888    888 888     888           888          Y88o88P   888     888 888    888 888     888 Y88b.
888Y88888P888 8888888        888     888   d88P 888     888           8888888       Y888P    888     888 888    888 888     888  "Y888b.
888 Y888P 888 888            888     8888888P"  888     888           888           d888b    888     888 888    888 888     888     "Y88b.
888  Y8P  888 888            888     888 T88b   888     888           888          d88888b   888     888 888    888 888     888       "888
888   "   888 888            888     888  T88b  Y88b. .d88P           888         d88P Y88b  Y88b. .d88P 888  .d88P Y88b. .d88P Y88b  d88P
888       888 8888888888     888     888   T88b  "Y88888P"            8888888888 d88P   Y88b  "Y88888P"  8888888P"   "Y88888P"   "Y8888P"

                                                                                                                       @github.com/sar0603)"
                << '\n';
    }

    void display_output() {
      system("dot -Tpng finalmap.dot -o path.png");
      system("gnome-terminal  -- sh -c \"fim --autowindow path.png\"");
    }

public:
    App() {
      system("notify-send -t 5000 -i ./train1.png \"metro Works\"");
      //sends app run notification to desktop
      system("gnome-terminal -x sh -c \"fim --autowindow graph.png\"");
      //opens the metro-station images represented as undirected graph with vertices as cities and dist as wts
      system("clear");
      system("sl");
      //fun train animation using built in sl command
      system("clear");
      system("echo  \"\e[96m\"");
    }

    void addCities() {//adds edges in program graph
      //red
      metro.addEdge("Rithala", "Netaji Subhash Place", 5.2);
      metro.addEdge("Netaji Subhash Place", "Keshav Puram", 1.2);
      metro.addEdge("Keshav Puram", "Kanhaiya Nagar", 0.8);
      metro.addEdge("Kanhaiya Nagar", "Inderlok", 1.2);
      metro.addEdge("Inderlok", "Shastri Nagar", 1.2);
      metro.addEdge("Shastri Nagar", "Pratap Nagar", 1.7);
      metro.addEdge("Pratap Nagar", "Pulbangash", 0.8);
      metro.addEdge("Pulbangash", "Tis Hazari", 0.9);
      metro.addEdge("Tis Hazari", "Kashmere Gate", 1.1);
      metro.addEdge("Kashmere Gate", "Shastri Park", 2.2);
      metro.addEdge("Shastri Park", "Seelampur", 1.6);
      metro.addEdge("Seelampur", "Welcome", 1.1);
      //blue
      metro.addEdge("Rajouri Garden", "Ramesh Nagar", 1);
      metro.addEdge("Ramesh Nagar", "Moti Nagar", 1.2);
      metro.addEdge("Moti Nagar", "Kirti Nagar", 1);
      metro.addEdge("Kirti Nagar", "Shadipur", 0.7);
      metro.addEdge("Shadipur", "Patel Nagar", 1.3);
      metro.addEdge("Patel Nagar", "Rajender Place", 0.9);
      metro.addEdge("Rajender Place", "Karol Bagh", 1);
      metro.addEdge("Karol Bagh", "Rajiv Chowk", 3.4);
      metro.addEdge("Rajiv Chowk", "Barakhamba Road", 0.7);
      metro.addEdge("Barakhamba Road", "Mandi House", 1);
      metro.addEdge("Mandi House", "Pragati Maiden", 0.8);
      metro.addEdge("Pragati Maiden", "Inderprastha", 0.8);
      metro.addEdge("Inderprastha", "Yamuna Bank", 1.8);
      metro.addEdge("Yamuna Bank", "Laxmi Nagar", 1.3);
      metro.addEdge("Laxmi Nagar", "Nirman Vihar", 1.1);
      metro.addEdge("Nirman Vihar", "Preet Vihar", 1.0);
      metro.addEdge("Preet Vihar", "Karkar Duma", 1.2);
      metro.addEdge("Karkar Duma", "Anand Vihar", 1.1);
      metro.addEdge("Anand Vihar", "Kaushambi", 0.8);
      metro.addEdge("Kaushambi", "Vaishali", 1.6);
      metro.addEdge("Yamuna Bank", "Akshardham", 1.3);
      metro.addEdge("Akshardham", "Mayur Vihar Phase-1", 1.8);
      metro.addEdge("Mayur Vihar Phase-1", "Mayur Vihar Extention", 1.2);
      metro.addEdge("Mayur Vihar Extention", "New Ashok Nagar", 0.9);
      metro.addEdge("New Ashok Nagar", "Noida Sector-15", 1.0);
      metro.addEdge("Noida Sector-15", "Noida Sector-16", 1.1);
      metro.addEdge("Noida Sector-16", "Noida Sector-18", 1.1);
      metro.addEdge("Noida Sector-18", "Botanical Garden", 1.1);
      metro.addEdge("Botanical Garden", "Golf Course", 1.2);
      metro.addEdge("Golf Course", "Noida City Center", 1.3);
      //green
      metro.addEdge("Madipur", "Shivaji Park", 1.1);
      metro.addEdge("Shivaji Park", "Punjabi Bagh", 1.6);
      metro.addEdge("Punjabi Bagh", "Ashok Park", 0.9);
      metro.addEdge("Ashok Park", "Inderlok", 1.4);
      metro.addEdge("Ashok Park", "Sant Guru Ram Singh Marg", 1.1);
      metro.addEdge("Sant Guru Ram Singh Marg", "Kirti Nagar", 1);
      metro.addEdge("Kashmere Gate", "Lal Qila", 1.5);
      metro.addEdge("Lal Qila", "Jama Masjid", 0.8);
      metro.addEdge("Jama Masjid", "Delhi Gate", 1.4);
      metro.addEdge("Delhi Gate", "ITO", 1.3);
      metro.addEdge("ITO", "Mandi House", 0.8);
      metro.addEdge("Mandi House", "Janptah", 1.4);
      metro.addEdge("Janptah", "Central Secretariat", 1.3);
      metro.addEdge("Central Secretariat", "Khan Market", 2.1);
      metro.addEdge("Khan Market", "JL Nehru Stadium", 1.4);
      metro.addEdge("JL Nehru Stadium", "Jangpura", 0.9);
      //yellow
      metro.addEdge("Vishwavidyalaya", "Vidhan Sabha", 1);
      metro.addEdge("Vidhan Sabha", "Civil Lines", 1.3);
      metro.addEdge("Civil Lines", "Kashmere Gate", 1.1);
      metro.addEdge("Kashmere Gate", "Chandni Chowk", 1.1);
      metro.addEdge("Chandni Chowk", "Chawri Bazar", 1);
      metro.addEdge("Chawri Bazar", "New Delhi", 0.8);
      metro.addEdge("New Delhi", "Rajiv Chowk", 1.1);
      metro.addEdge("Rajiv Chowk", "Patel Chowk", 1.3);
      metro.addEdge("Patel Chowk", "Central Secretariat", 0.9);
      metro.addEdge("Central Secretariat", "Udyog Bhawan", 0.3);
      metro.addEdge("Udyog Bhawan", "Lok Kalyan Marg", 1.6);
      metro.addEdge("Lok Kalyan Marg", "Jor Bagh", 1.2);
      metro.addEdge("Samaypur Badli", "Rohini Sector - 18", 0.8);
      metro.addEdge("Rohini Sector - 18", "Haiderpur Badli Mor", 1.3);
      metro.addEdge("Haiderpur Badli Mor", "Jahangirpuri", 1.3);
      metro.addEdge("Jahangirpuri", "Adarsh Nagar", 1.3);
      metro.addEdge("Adarsh Nagar", "Azadpur", 1.5);
      metro.addEdge("Azadpur", "Model Town", 1.4);
      metro.addEdge("Model Town", "GTB Nagar", 1.4);
      metro.addEdge("GTB Nagar", "Vishwavidyalaya", 0.8);
      metro.addEdge("Jor Bagh", "INA", 1.3);
      metro.addEdge("INA", "AIIMS", 0.8);
      metro.addEdge("AIIMS", "Green Park", 1.0);
      metro.addEdge("Green Park", "Hauz Khas", 1.8);
      metro.addEdge("Hauz Khas", "Malviya Nagar", 1.7);
      metro.addEdge("Malviya Nagar", "Saket", 0.9);
      metro.addEdge("Saket", "Qutab Minar", 1.7);
      metro.addEdge("Qutab Minar", "Chhattarpur", 1.3);
      metro.addEdge("Chhattarpur", "Sultanpur", 1.6);
      metro.addEdge("Sultanpur", "Ghitorni", 1.3);
      metro.addEdge("Ghitorni", "Arjan Garh", 2.7);
      metro.addEdge("Arjan Garh", "Guru Dronacharya", 2.3);
      metro.addEdge("Guru Dronacharya", "Sikandarpur", 1.0);
      metro.addEdge("Sikandarpur", "MG Road", 1.2);
      metro.addEdge("MG Road", "IFFCO Chowk", 1.1);
      metro.addEdge("IFFCO Chowk", "Huda City Centre", 1.5);
    }

    void run() {
      display_welcome();
      map<ver, float> dist;
      map<ver, ver> prev;
      string src, des;
      get_input(src, des);

      bool res = metro.print_adj(src, des);
      if (!res) {//error handling
        handle_error();
        return;
      }
      metro.dijkstra_sssp(src, dist, prev);
      cout << endl << "\t\t";
      cout << "Distance from " << src << " to " << des << " - " << dist[des] << " Kms" << endl;
      cout << endl << "\t\tPath: " << endl;
      metro.dijkstra_get_sssp(des, prev);
      metro.make_dot_file();
      metro.calc_price(src, des);
      display_output();
    }
};

int main() {
  App metro_exodus;
  metro_exodus.addCities();
  metro_exodus.run();
  return 0;
}