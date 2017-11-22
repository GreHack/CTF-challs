package main

import (
	"fmt"
	"github.com/tebeka/selenium"
	"math"
	"net/http"
	"time"
)

func executor(id string) {
	caps := selenium.Capabilities{"browserName": "firefox"}
	wd, err := selenium.NewRemote(caps, "http://firefox:4444/wd/hub")
	if err != nil {
		fmt.Printf("Failed to open session: %s\n", err)
		return
	}
	defer wd.Quit()

	err = wd.Get("http://web/")
	if err != nil {
		fmt.Printf("Failed to load page: %s\n", err)
		return
	}

	adminCookie := &selenium.Cookie{
		Name:   "araas_session_admin",
		Value:  "9bBnEQ%k#Yb^2&c@M96f5AQrf2Ae7C",
		Expiry: math.MaxUint32,
	}
	if err := wd.AddCookie(adminCookie); err != nil {
		fmt.Printf("Failed to set admin cookie:  %s\n", err)
	}

	err = wd.Get("http://web/admin.php?id=" + id)
	if err != nil {
		fmt.Printf("Failed to load page: %s\n", err)
		return
	}

	button, err := wd.FindElement(selenium.ByID, "submit")
	if err != nil {
		fmt.Printf("Failed to get submit button: %s\n", err)
		return
	}

	time.Sleep(time.Millisecond * 3000)

	if err := button.SendKeys(selenium.EnterKey); err != nil {
		fmt.Printf("Failed to submit form: %s\n", err)
		return
	}
}

func handler(w http.ResponseWriter, r *http.Request) {
	id := r.FormValue("id")
	fmt.Println("Processing", id)
	go executor(id)
}

func main() {
	http.HandleFunc("/", handler)
	http.ListenAndServe(":8080", nil)
}
