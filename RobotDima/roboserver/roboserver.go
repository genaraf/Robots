package main

import (
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os/exec"
	"sync"

	"github.com/gorilla/mux"
)

var report string
var mutex = &sync.Mutex{}

func addReport(rep string) {
	mutex.Lock()
	report += rep
	fmt.Printf("%s\n", rep)
	mutex.Unlock()
}
func getProgName(id string) string {
	return "./prog/program-" + id + ".r"
}

func executeProgram(progName string) {
	// execute program
	cmd := exec.Command("./robocomp", progName)
	stderr, err := cmd.StdoutPipe()
	if err != nil {
		log.Println(err)
		return
	}

	if err := cmd.Start(); err != nil {
		log.Println(err)
		return
	}

	slurp, _ := ioutil.ReadAll(stderr)
	addReport(string(slurp))

	if err := cmd.Wait(); err != nil {
		log.Println(err)
		return
	}
}

func main() {
	r := mux.NewRouter()
	r.HandleFunc("/view/program/{id}", func(w http.ResponseWriter, r *http.Request) {
		vars := mux.Vars(r)
		fileName := getProgName(vars["id"])
		log.Print("view program:", fileName)
		http.ServeFile(w, r, fileName)
	})

	r.HandleFunc("/save/program/{id}", func(w http.ResponseWriter, r *http.Request) {
		vars := mux.Vars(r)
		fileName := getProgName(vars["id"])
		log.Print("save program:", fileName)
		b, err := ioutil.ReadAll(r.Body)
		defer r.Body.Close()
		if err != nil {
			http.Error(w, err.Error(), 500)
			return
		}
		log.Print("save program:", string(b))
		err = ioutil.WriteFile(fileName, b, 0644)
		if err != nil {
			http.Error(w, err.Error(), 501)
			return
		}
		fmt.Fprintf(w, "program: "+fileName+" saved\n")
	})

	r.HandleFunc("/run/program/{id}", func(w http.ResponseWriter, r *http.Request) {
		vars := mux.Vars(r)
		fileName := getProgName(vars["id"])
		log.Print("save program:", fileName)
		b, err := ioutil.ReadAll(r.Body)
		defer r.Body.Close()
		if err != nil {
			http.Error(w, err.Error(), 500)
			return
		}
		err = ioutil.WriteFile(fileName, b, 0644)
		if err != nil {
			http.Error(w, err.Error(), 501)
			return
		}

		log.Print("run program:", fileName)

		// kill previous instance
		cmd := exec.Command("killall", "-9", "robocomp")
		log.Printf("Running command and waiting for it to finish...")
		err = cmd.Run()
		log.Printf("Command finished with error: %v", err)
		fmt.Fprintf(w, "execute program: "+fileName+"\n")
		go executeProgram(fileName)
	})

	r.HandleFunc("/stop/program", func(w http.ResponseWriter, r *http.Request) {
		// kill previous instance
		cmd := exec.Command("killall", "-9", "robocomp")
		log.Printf("Running command and waiting for it to finish...")
		err := cmd.Run()
		fmt.Fprintf(w, "Programm stopped\n")
		log.Printf("Command finished with error: %v", err)

	})

	r.HandleFunc("/report", func(w http.ResponseWriter, r *http.Request) {
		mutex.Lock()
		if len(report) > 0 {
			fmt.Fprintf(w, report)
			report = ""
		}
		mutex.Unlock()
	})

	r.PathPrefix("/").Handler(http.StripPrefix("/", http.FileServer(http.Dir("./web/"))))

	http.ListenAndServe(":8080", r)
}
