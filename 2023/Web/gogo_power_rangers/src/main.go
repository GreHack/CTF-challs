package main

import (
	"bytes"
	"html/template"
	"log"
	"net/http"
	"os"
	"runtime/debug"

	"github.com/golang-jwt/jwt"
	"github.com/joho/godotenv"
)

type Claims struct {
	Color string `json:"color"`
	jwt.StandardClaims
}

type dummyObj struct {
	data string
}

func (obj *dummyObj) Readenv(key string) string {
	return os.Getenv(key)
}

func recoveryMiddleware(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		defer func() {
			if err := recover(); err != nil {
				log.Printf("Panic recovered: %v", err)
				debugStack := string(debug.Stack())
				log.Printf("Stack Trace: \n%s\n", debugStack)
				http.Error(w, "Internal Server Error", http.StatusInternalServerError)
			}
		}()
		next.ServeHTTP(w, r)
	})
}

func ColorSelection(w http.ResponseWriter, r *http.Request) {
	// debug
	err := r.ParseForm()
	if err != nil {
		log.Println("Failed to parse form:", err)
		w.WriteHeader(http.StatusBadRequest)
		return
	}

	if r.Method == "POST" {
		color := r.FormValue("color")
		claims := &Claims{
			Color: color,
		}

		token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
		tokenString, err := token.SignedString([]byte(os.Getenv("JWT_SECRET")))
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		http.SetCookie(w, &http.Cookie{
			Name:  "token",
			Value: tokenString,
		})

		// Add the personalized message
		var sentence string
		switch color {
		case "red":
			sentence = "Strength and Power!"
		case "blue":
			sentence = "Wisdom and Intelligence!"
		case "yellow":
			sentence = "Compassion and Speed!"
		case "green":
			sentence = "Mystery and Stealth!"
		case "pink":
			sentence = "Grace and Flexibility!"
		default:
			sentence = "Welcome, " + color + " Ranger!"
		}

		w.Write([]byte(sentence))

	} else if r.Method == "GET" {
		c, err := r.Cookie("token")
		if err != nil {
			if err == http.ErrNoCookie {
				t, _ := template.ParseFiles("colors.html")
				t.Execute(w, nil)
				return
			}
			w.WriteHeader(http.StatusBadRequest)
			return
		}

		tknStr := c.Value
		claims := &Claims{}

		tkn, err := jwt.ParseWithClaims(tknStr, claims, func(token *jwt.Token) (interface{}, error) {
			return []byte(os.Getenv("JWT_SECRET")), nil
		})

		if err != nil {
			if err == jwt.ErrSignatureInvalid {
				w.WriteHeader(http.StatusUnauthorized)
				return
			}
			w.WriteHeader(http.StatusBadRequest)
			return
		}

		if !tkn.Valid {
			w.WriteHeader(http.StatusUnauthorized)
			return
		}

		dumObj := &dummyObj{
			data: "data",
		}

		colorTemplate := claims.Color

		tmpl, err := template.New("color").Parse(colorTemplate)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		var tpl bytes.Buffer
		log.Println(dumObj)
		err = tmpl.Execute(&tpl, dumObj)
		if err != nil {
			w.WriteHeader(http.StatusInternalServerError)
			return
		}

		parsedColorTemplate := tpl.String()

		data := struct {
			Greeting            string
			dumObj              *dummyObj
			ParsedColorTemplate string
		}{
			Greeting:            "Welcome, Ranger!",
			dumObj:              dumObj,
			ParsedColorTemplate: parsedColorTemplate,
		}

		t, _ := template.ParseFiles("colors.html")
		t.ExecuteTemplate(w, "colors.html", data)
	}
}

func main() {
	err := godotenv.Load()
	if err != nil {
		panic("Failed to load env file")
	}

	port := os.Getenv("PORT")
	if port == "" {
		log.Fatal("$PORT must be set")
	}

	http.HandleFunc("/", ColorSelection)
	http.Handle("/static/", http.StripPrefix("/static/", http.FileServer(http.Dir("static"))))

	http.ListenAndServe(":"+port, recoveryMiddleware(http.DefaultServeMux))
}
