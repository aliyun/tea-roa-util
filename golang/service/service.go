package service

import (
	"bytes"
	"crypto/hmac"
	"crypto/sha1"
	"encoding/base64"
	"hash"
	"io"
	"sort"
	"strings"

	"github.com/alibabacloud-go/tea/tea"
)

type Sorter struct {
	Keys []string
	Vals []string
}

func newSorter(m map[string]string) *Sorter {
	hs := &Sorter{
		Keys: make([]string, 0, len(m)),
		Vals: make([]string, 0, len(m)),
	}

	for k, v := range m {
		hs.Keys = append(hs.Keys, k)
		hs.Vals = append(hs.Vals, v)
	}
	return hs
}

// Sort is an additional function for function SignHeader.
func (hs *Sorter) Sort() {
	sort.Sort(hs)
}

// Len is an additional function for function SignHeader.
func (hs *Sorter) Len() int {
	return len(hs.Vals)
}

// Less is an additional function for function SignHeader.
func (hs *Sorter) Less(i, j int) bool {
	return bytes.Compare([]byte(hs.Keys[i]), []byte(hs.Keys[j])) < 0
}

// Swap is an additional function for function SignHeader.
func (hs *Sorter) Swap(i, j int) {
	hs.Vals[i], hs.Vals[j] = hs.Vals[j], hs.Vals[i]
	hs.Keys[i], hs.Keys[j] = hs.Keys[j], hs.Keys[i]
}

func Is4XXor5XX(code int) bool {
	return code >= 400 && code < 600
}

func GetSignature(stringToSign string, secret string) string {
	h := hmac.New(func() hash.Hash { return sha1.New() }, []byte(secret))
	io.WriteString(h, stringToSign)
	signedStr := base64.StdEncoding.EncodeToString(h.Sum(nil))
	return signedStr
}

func GetStringToSign(request *tea.Request) string {
	return getStringToSign(request)
}

func getStringToSign(request *tea.Request) string {
	resource := request.Pathname
	tmp := ""
	for key, value := range request.Query {
		if value != "" {
			tmp = tmp + "&" + key + "=" + value
		}
	}
	if tmp != "" {
		resource = resource + "?" + tmp
	}
	return getSignedStr(request, resource)
}

func getSignedStr(req *tea.Request, canonicalizedResource string) string {
	temp := make(map[string]string)

	for k, v := range req.Headers {
		if strings.HasPrefix(strings.ToLower(k), "x-acs-") {
			temp[strings.ToLower(k)] = v
		}
	}
	hs := newSorter(temp)

	// Sort the temp by the ascending order
	hs.Sort()

	// Get the canonicalizedOSSHeaders
	canonicalizedOSSHeaders := ""
	for i := range hs.Keys {
		canonicalizedOSSHeaders += hs.Keys[i] + ":" + hs.Vals[i] + "\n"
	}

	// Give other parameters values
	// when sign URL, date is expires
	date := req.Headers["date"]
	accept := req.Headers["accept"]
	contentType := req.Headers["content-type"]
	contentMd5 := req.Headers["content-md5"]

	signStr := req.Method + "\n" + accept + "\n" + contentMd5 + "\n" + contentType + "\n" + date + "\n" + canonicalizedOSSHeaders + canonicalizedResource
	return signStr
}

func DeleteSpecialKey(obj map[string]interface{}, key string) map[string]interface{} {
	res := make(map[string]interface{})
	for k, v := range obj {
		if key != k {
			res[k] = v
		}
	}
	return res
}
