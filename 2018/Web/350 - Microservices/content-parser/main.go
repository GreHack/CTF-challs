package main

import (
	"encoding/json"
	"fmt"
	"gopkg.in/src-d/go-git.v4/plumbing/protocol/packp"
	"net/http"
)

type Message struct {
	Name string
	Body string
	Time int64
}

func main() {
	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
		ar := packp.NewAdvRefs()
		err := ar.Decode(r.Body)
		if err != nil {
			http.Error(w, err.Error(),  http.StatusBadRequest)
			return
		}

		m := make(map[string]string)
		for ref, h := range ar.References {
			m[ref] = h.String()
		}

		j, _ := json.Marshal(m)
		if err != nil {
			http.Error(w, err.Error(), http.StatusInternalServerError)
			return
		}

		w.Header().Set("Content-Type", "application/json")
		fmt.Fprint(w, string(j))
	})

	http.ListenAndServe(":8082", nil)
}