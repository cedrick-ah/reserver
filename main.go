package main

import "C"

import (
	"fmt"
	"log"
	"net/http"

	"github.com/gorilla/websocket"
)

var upgrader = websocket.Upgrader{}
var ws *websocket.Conn

//export serve
func serve(){
    fileServer := http.FileServer(http.Dir("./static"))
    http.Handle("/", fileServer)

    http.HandleFunc("/echo", func(w http.ResponseWriter, r *http.Request) {
        conn, err := upgrader.Upgrade(w, r, nil)
        if err != nil {
            log.Print("Upgrade failed: ", err)
            return
        }
        defer conn.Close()

        ws = conn
        for {
            _, message, err := conn.ReadMessage()
            if err != nil {
                log.Println("Read failed:", err)
                break
            }
            input := string(message)
            fmt.Printf(input)
        }
    })

    fmt.Printf("Starting server at port 8080\n")
    fmt.Printf("Waiting for the browser connection...\n")

    if err := http.ListenAndServe(":8080", nil); err != nil {
        log.Fatal(err);
    }
}

//export update
func update() {
    var output = "reload"
    message := []byte(output)
    if ws != nil {
        err := ws.WriteMessage(1, message)
        if err != nil {
            log.Println("Write failed:", err)
        }
    }else {
        fmt.Printf("No browser attached\n")
    }
}

func main(){
}
