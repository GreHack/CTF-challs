package main

import (
	"context"
	"errors"
	"fmt"
	"github.com/chromedp/cdproto/cdp"
	"github.com/chromedp/cdproto/network"
	"github.com/chromedp/chromedp"
	"github.com/chromedp/chromedp/runner"
	"github.com/go-redis/redis"
	"os"
	"strconv"
	"time"
)

func main() {
	maxBrowsers := 6
	envMaxBrowsers, exists := os.LookupEnv("MAX_BROWSERS")
	if exists {
		if s, err := strconv.ParseInt(envMaxBrowsers, 10, 32); err == nil {
			maxBrowsers = int(s)
		}
	}

	redisdb := redis.NewClient(&redis.Options{
		Addr:     "internal-resources-redis:6379",
		Password: "EJH8Xv1jeZKRjN66F4PcQA4AE65592pF",
		DB:       0,
	})
	defer redisdb.Close()

	port := 9222

	for {
		result, err := redisdb.BRPop(0, "browse").Result()
		if err != nil {
			fmt.Println(err)
			continue
		}

		fmt.Printf("Waiting to process %s\n", result[1])
		if port > 9600 {
			port = 9222
		} else {
			port += 1
		}
		go func(url string, port int) {
			fmt.Printf("Start processing %s\n", url)

			ctxt, cancel := context.WithCancel(context.Background())
			defer cancel()

			c, err := chromedp.New(ctxt, chromedp.WithRunnerOptions(
				runner.Flag("no-sandbox", true),
				runner.Flag("headless", true),
				runner.Flag("disable-gpu", true),
				runner.Flag("remote-debugging-port", port),
			))
			if err != nil {
				fmt.Println(err)
			}
			err = c.Run(ctxt, browse(url))
			if err != nil {
				fmt.Println(err)
			}

			err = c.Shutdown(ctxt)
			if err != nil {
				fmt.Println(err)
			}
			fmt.Printf("End of processing for %s\n", url)
			//err = c.Wait() // Hangs in headless mode
			//if err != nil {
			//	fmt.Println(err)
			//}
		}(result[1], port)

		maxBrowsers -= 1
		if maxBrowsers < 0 {
			time.Sleep(20 * time.Second)
			fmt.Println("Shuting down")
			return
		}
	}
}

func browse(url string) chromedp.Tasks {
	return chromedp.Tasks{
		chromedp.ActionFunc(func(ctxt context.Context, h cdp.Executor) error {
			expr := cdp.TimeSinceEpoch(time.Now().Add(24 * time.Hour))
			success, err := network.SetCookie("admin", "bDj9qE@r4aMxUZPKKjh#zcE8!u4m5gN4").
				WithExpires(&expr).
				WithDomain("admin.internal.test").
				WithHTTPOnly(true).
				Do(ctxt, h)
			if err != nil {
				return err
			}
			if !success {
				return errors.New("could not set cookie")
			}
			return nil
		}),
		chromedp.Navigate(url),
		chromedp.ActionFunc(func(context.Context, cdp.Executor) error {
			time.Sleep(20 * time.Second)
			return nil
		}),
	}
}
