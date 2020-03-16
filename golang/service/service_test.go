package service

import (
	"testing"

	"github.com/alibabacloud-go/tea/tea"
	"github.com/alibabacloud-go/tea/utils"
)

func Test_GetSignature(t *testing.T) {
	request := tea.NewRequest()
	sign := GetStringToSign(request)
	signature := GetSignature(sign, "secret")
	utils.AssertEqual(t, 28, len(signature))
}

func Test_Sorter(t *testing.T) {
	tmp := map[string]string{
		"key":   "ccp",
		"value": "ok",
	}
	sort := newSorter(tmp)
	sort.Sort()

	len := sort.Len()
	utils.AssertEqual(t, len, 2)

	isLess := sort.Less(0, 1)
	utils.AssertEqual(t, isLess, true)

	sort.Swap(0, 1)
	isLess = sort.Less(0, 1)
	utils.AssertEqual(t, isLess, false)
}

func Test_getStringToSign(t *testing.T) {
	request := tea.NewRequest()
	request.Query = map[string]string{
		"ccp": "ok",
	}
	request.Headers = map[string]string{
		"x-acs-meta": "user",
	}
	str := getStringToSign(request)
	utils.AssertEqual(t, 28, len(str))
}
