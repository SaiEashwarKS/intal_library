#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int maxi(int a, int b) //returns maximum of two integers
{
    return a >= b ? a : b;
}

static int mini(int a, int b) //returns minimum of two integers
{
    return a <= b ? a : b;
}

static int absolute(int a) //returns the absolute value of an integer
{
    return a > 0 ? a : (-1 * a);
}

static char *strip(char *a) //removes leading zeros in an intal
{
    int offset = 0;
    while (a[offset] == '0') //finds the number of leading zeros and stores it in 'offset'
    {
        ++offset;
    }
    if (offset == 0) //no leading zero
                     //just copy the original intal into temp and return temp because return type is char*
                     //(and we dont want to return the original intal itself..
                     //..because the client might choose to free the original intal)
    {
        char *temp = (char *)malloc(sizeof(char) * (strlen(a) + 1)); //+1 to store the null character
        strcpy(temp, a);
        temp[strlen(a)] = '\0';
        return temp;
    }
    int diff = absolute(strlen(a) - offset); //number of digits without the leading zeros
    char *temp = (char *)malloc(sizeof(char) * (diff + 1));
    for (int i = 0; i < diff; ++i)
    {
        temp[i] = a[i + offset];
    }
    temp[diff] = '\0';
    return temp;
}

char *intal_add(const char *intal1, const char *intal2)
{
    if(strcmp(intal1, "0") == 0 && strcmp(intal2, "0") == 0)
    {
        char *res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    int carry = 0;                                            //to store intermediate carry bits during addition
    int min_len = mini(strlen(intal1), strlen(intal2));       //length of the smaller intal
    int max_len = maxi(strlen(intal1), strlen(intal2));       //length of the bigger intal
    int diff = max_len - min_len;                             //difference in the number of digits of the two intals
    int res_len = 0;                                          //length of the resulting intal
    char *res = (char *)malloc(sizeof(char) * (max_len + 2)); //+2 because addition of two intals might result in an intal..
                                                              //..which has one digit more than the bigger intal
    int temp_sum = 0;                                         //will contain the intermediate sums of two corresponding digits of the two intals
    int temp_sum_units;                                       //will contain the digit in units place in temp_sum
    if (strlen(intal1) == max_len)                            //if intal1 has more digits
    {
        //Note : (int)('<digit>') - (int)('0') gives us the digit in integer form. Can use atoi(string) alternatively
        for (int i = min_len - 1; i >= 0; --i)
        {
            temp_sum = (int)(intal1[diff + i]) - (int)('0') + (int)(intal2[i]) - (int)('0') + carry;
            temp_sum_units = temp_sum % 10;
            res[max_len - res_len] = (char)(temp_sum_units + (int)('0')); //storing the character version of the temp_sum_units
            carry = temp_sum / 10;                                        //Note: Sum of two digits can not be greater than 18. Therefore carry will be either 0 or 1
            ++res_len;
        }
        for (int i = diff - 1; i >= 0; --i)
        {
            temp_sum = (int)(intal1[i]) - (int)('0') + carry;
            temp_sum_units = temp_sum % 10;
            res[max_len - res_len] = (char)(temp_sum_units + (int)('0')); //storing the character version of the temp_sum_units
            carry = temp_sum / 10;                                        //Note: Sum of two digits can not be greater than 18. Therefore carry will be either 0 or 1
            ++res_len;
        }
    }
    else //if intal2 has more digits or both intals are of equal length
    {
        for (int i = min_len - 1; i >= 0; --i)
        {
            temp_sum = (int)(intal2[diff + i]) - (int)('0') + (int)(intal1[i]) - (int)('0') + carry;
            temp_sum_units = temp_sum % 10;
            res[max_len - res_len] = (char)(temp_sum_units + (int)('0')); //storing the character version of the temp_sum_units
            carry = temp_sum / 10;                                        //Note: Sum of two digits can not be greater than 18. Therefore carry will be either 0 or 1
            ++res_len;
        }
        for (int i = diff - 1; i >= 0; --i)
        {
            temp_sum = (int)(intal2[i]) - (int)('0') + carry;
            temp_sum_units = temp_sum % 10;
            res[max_len - res_len] = (char)(temp_sum_units + (int)('0')); //storing the character version of the temp_sum_units
            carry = temp_sum / 10;                                        //Note: Sum of two digits can not be greater than 18. Therefore carry will be either 0 or 1
            ++res_len;
        }
    }
    if (carry == 1)
    {
        res[0] = '1';
        ++res_len;
    }
    else
    {
        res[0] = '0';
        ++res_len;
    }
    res[res_len] = '\0';
    if (res[0] == '0') //if res has leading zeros
    {
        char *temp_free = (char *)malloc(sizeof(char) * (strlen(res) + 1));
        temp_free = res; //storing the res with leading zeros to free later
        res = strip(res);
        free(temp_free); //freeing the previous value of res which had leading zeros
    }
    return res;
}

int intal_compare(const char *intal1, const char *intal2)
{
    if (strlen(intal1) != strlen(intal2))
    {
        return strlen(intal1) > strlen(intal2) ? 1 : -1;
    }
    int i = 0;
    while (i < strlen(intal1))
    {
        if (intal1[i] != intal2[i])
        {
            return intal1[i] > intal2[i] ? 1 : -1;
        }
        ++i;
    }
    return 0;
}

char *intal_diff(const char *intal1, const char *intal2)
{
    if(strcmp(intal1, "0") == 0 && strcmp(intal2, "0") == 0)
    {
        char *res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    int compare = intal_compare(intal1, intal2);
    if (compare == 0) //if both intals are equal, should return '0'
    {
        char *res = (char *)malloc(sizeof(char) * 2); //2 to store '0' and null character
        strcpy(res, "0");
        return res;
    }
    int diff = absolute(strlen(intal1) - strlen(intal2)); //difference in the number of digits of the two intals
    int res_len = 0;                                      //length of the resulting intal
    int borrow = 0;                                       //to store intermediate borrow bits during addition
    int temp_diff;                                        //will contain the intermediate differences of two corresponding digits of the two intals
    char *res;
    int flag = 0;     //resulting intal
    if (compare == 1) //if intal1 is greater
    {
        res = (char *)malloc(sizeof(char) * (strlen(intal1) + 1));
        for (int i = strlen(intal1) - 1; i >= diff; --i) //subtracting corresponding digits starting from units place
        {
            if ((int)intal1[i] - borrow < (int)intal2[i - diff]) //should borrow from the digit on the left
            {
                temp_diff = 10 + (int)intal1[i] - (int)intal2[i - diff] - borrow; //10 because 1 is borrowed from the digit on the left
                                                                                  // -borrow because the digit might have given a 1 to the..
                                                                                  //..digit to its right (borrow will be 1 then, 0 otherwise)
                borrow = 1;                                                       //because 1 is borrowed from the digit on the left
            }
            else
            {
                temp_diff = (int)intal1[i] - (int)intal2[i - diff] - borrow; //no need to borrow from the digit on the left
                borrow = 0;                                                  ///because 1 is not borrowed
            }
            res[i] = (char)(temp_diff + (int)('0')); //storing the character version of the difference
            //(temp_diff is always a single digit because difference between two digits is a single digit even if 1 is borrowed)
            ++res_len;
        }
        //when corresponding digits from the two intals are subtracted, the last digit, i.e the digit at index 'diff' might have..
        //..borrowed a 1 from the digit to its left. Therefore we cannot directly store the digit at index 'diff-1' to res
        if (diff > 0)
            flag = 1;
        while (flag && diff >= 0)
        {
            if (borrow == 1 && intal1[diff - 1] != '0')
            {
                temp_diff = (int)intal1[diff - 1] - borrow;
                --diff;
                flag = 0;
            }
            else if (borrow == 1 && intal1[diff - 1] == '0')
            {
                temp_diff = 57; //10 + (int)intal1[diff - 1] - borrow;
                --diff;
            }
            else
            {
                temp_diff = (int)intal1[diff - 1];
                --diff;
                flag = 0;
            }
            if (diff >= 0)
            {
                res[diff] = (char)(temp_diff);
                ++res_len;
            }
        }
        //the other excess digits in intal1 (which is the longer intal) can be directly copied to res
        for (int i = 0; i < diff; ++i)
        {
            res[i] = intal1[i];
            ++res_len;
        }
    }
    else //similar to the case when intal1 is greater except here, intal2 > intal1
    {
        res = (char *)malloc(sizeof(char) * (strlen(intal2) + 1));
        for (int i = strlen(intal2) - 1; i >= diff; --i)
        {
            if ((int)intal2[i] - borrow < (int)intal1[i - diff])
            {
                temp_diff = 10 + (int)intal2[i] - (int)intal1[i - diff] - borrow;
                borrow = 1;
            }
            else
            {
                temp_diff = (int)intal2[i] - (int)intal1[i - diff] - borrow;
                borrow = 0;
            }
            res[i] = (char)(temp_diff + (int)('0'));
            //printf("res[%d] : %c ", i, res[i]);
            ++res_len;
        }
        if (diff > 0)
            flag = 1;
        while (flag && diff >= 0)
        {
            if (borrow == 1 && intal2[diff - 1] != '0')
            {
                temp_diff = (int)intal2[diff - 1] - borrow;
                --diff;
                flag = 0;
            }
            else if (borrow == 1 && intal2[diff - 1] == '0')
            {
                temp_diff = 57; //10 + (int)intal1[diff - 1] - borrow;
                --diff;
            }
            else
            {
                temp_diff = (int)intal2[diff - 1];
                --diff;
                flag = 0;
            }
            if (diff >= 0)
            {
                res[diff] = (char)(temp_diff);
                ++res_len;
            }
        }
        //the other excess digits in intal1 (which is the longer intal) can be directly copied to res
        for (int i = 0; i < diff; ++i)
        {
            res[i] = intal2[i];
            ++res_len;
        }
    }
    res[res_len] = '\0';
    if (res[0] == '0') //remove leading zeros if present
    {
        char *temp_free = (char *)malloc(sizeof(char) * (res_len + 1));
        temp_free = res;
        res = strip(res);
        free(temp_free);
    }
    return res;
}

char *intal_multiply(const char *intal1, const char *intal2)
{
    if (intal1[0] == '0' || intal2[0] == '0')
    {
        char *res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    int l1 = strlen(intal1);
    int l2 = strlen(intal2);
    int res_len = l1 + l2;                                    //product of two numbers having m and n digits will have m+n-1 or m+n digits
    char *res = (char *)malloc(sizeof(char) * (res_len + 1)); //+1 to account for null character
    for (int i = 0; i < res_len + 1; ++i)
    {
        res[i] = '0';
    }
    res[res_len] = '\0';
    int carry, temp_res, temp_prod, temp_sum;
    carry = 0;
    for (int i = 0; i < l1; ++i)
    {
        for (int j = 0; j < l2; ++j)
        {
            temp_prod = ((int)intal1[l1 - 1 - i] - 48) * ((int)intal2[l2 - 1 - j] - 48);
            temp_res = (int)(res[res_len - 1 - i - j] - 48);
            temp_sum = temp_prod + temp_res + carry;
            res[res_len - 1 - i - j] = (char)(temp_sum % 10 + 48);
            carry = temp_sum / 10;
        }
        for (int j = l2 + i; j < l2 + l1; ++j)
        {
            temp_res = (int)(res[res_len - 1 - j] - 48);
            temp_sum = temp_res + carry;
            res[res_len - 1 - j] = (char)(temp_sum % 10 + 48);
            carry = temp_sum / 10;
        }
    }
    if (res[0] == '0')
    {
        char *temp;
        temp = res;
        res = strip(temp);
        free(temp);
    }
    return res;
}

char *intal_mod(const char *intal1, const char *intal2)
{
    int l1 = strlen(intal1);
    int l2 = strlen(intal2);
    int compare = intal_compare(intal1, intal2);
    char *res;
    if (compare == 0)
    {
        res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    else if (compare == -1)
    {
        res = (char *)malloc(sizeof(char) * (l1 + 1));
        strcpy(res, intal1);
        return res;
    }
    //long division method
    char *dividend = (char *)malloc(sizeof(char) * (l2 + 2));
    for (int i = 0; i < l2; ++i)
    {
        dividend[i] = intal1[i];
    }
    dividend[l2] = '\0';
    int dividend_len = l2;
    do
    {

        if (intal_compare(dividend, intal2) == -1 && dividend_len < l1)
        {

            if (strcmp(dividend, "0") == 0)
            {
                dividend[0] = intal1[dividend_len];
                dividend[1] = '\0';
            }
            else
            {
                int length = strlen(dividend);
                dividend[length] = intal1[dividend_len];
                dividend[length + 1] = '\0';
            }
            ++dividend_len;
        }
        if (intal_compare(dividend, intal2) != -1)
        {
            char *diff = intal_diff(dividend, intal2);
            while (intal_compare(diff, intal2) > -1)
            {
                char *temp = diff;
                diff = intal_diff(diff, intal2);
                free(temp);
            }
            strcpy(dividend, diff);
            free(diff);
            //printf("%s ", dividend);
        }

    } while (dividend_len < l1); //do-while because we have to perform this loop once even if the..
                                 //..lengths of intal1 and intal2 are same
    dividend = realloc(dividend, sizeof(char) * (strlen(dividend) + 1));
    dividend[strlen(dividend) + 1] = '\0';
    return dividend;
}

char *intal_pow(const char *intal1, unsigned int n)
{
    if (intal1[0] == '0')
    {
        char *res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    if (n == 0)
    {
        char *res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "1");
        return res;
    }
    char *base = (char *)malloc(sizeof(char) * (strlen(intal1) + 1));
    char *temp;
    char *res = (char *)malloc(sizeof(char) * (2));
    strcpy(res, "1");
    strcpy(base, intal1);
    while (n > 0)
    {
        if (n % 2 == 0)
        {
            n = n / 2;
            temp = base;
            base = intal_multiply(base, base);
            free(temp);
        }
        else
        {
            n -= 1;
            temp = res;
            res = intal_multiply(res, base);
            free(temp);
            n = n / 2;
            temp = base;
            base = intal_multiply(base, base);
            free(temp);
        }
    }
    return res;
}

char *intal_gcd(const char *intal1, const char *intal2)
{
    char *res;
    if (strcmp(intal1, "0") == 0 && strcmp(intal2, "0") == 0)
    {
        res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "0");
        return res;
    }
    if (strcmp(intal1, "0") == 0)
    {
        res = (char *)malloc(sizeof(char) * (strlen(intal2) + 1));
        strcpy(res, intal2);
        return res;
    }
    if (strcmp(intal2, "0") == 0)
    {
        res = (char *)malloc(sizeof(char) * (strlen(intal1) + 1));
        strcpy(res, intal1);
        return res;
    }
    char *a = (char *)malloc(sizeof(char) * (strlen(intal1) + 1));
    char *b = (char *)malloc(sizeof(char) * (strlen(intal2) + 1));
    char *temp;
    char *remainder;
    strcpy(a, intal1);
    strcpy(b, intal2);
    int compare = intal_compare(b, "0");
    while (compare == 1)
    {
        //printf("%s %s\n", a, b);
        remainder = intal_mod(a, b);
        temp = a;
        a = b;
        free(temp);
        b = remainder;
        compare = intal_compare(b, "0");
    }
    return a;
}

char *intal_fibonacci(unsigned int n)
{
    char *a;
    char *b;
    char *c;
    char *temp;
    a = (char *)malloc(sizeof(char) * 2);
    b = (char *)malloc(sizeof(char) * 2);
    //c = (char*)malloc(sizeof(char) * 2);
    strcpy(a, "0");
    strcpy(b, "1");
    c = intal_add(a, b);
    if (n == 0)
    {
        return a;
    }
    if (n == 1)
    {
        return b;
    }
    int i = 2;
    while (i < n)
    {
        temp = a;
        a = b;
        free(temp);

        b = c;
        //temp = c;
        c = intal_add(a, b);
        //free(temp);

        ++i;
    }
    free(a);
    free(b);
    return c;
}

char *intal_factorial(unsigned int n)
{
    char *res;
    if (n == 0 || n == 1)
    {
        res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, "1");
        return res;
    }
    int i = 1;
    char *num;
    num = (char *)malloc(sizeof(char) * 2);
    strcpy(num, "1");
    res = (char *)malloc(sizeof(char) * 2);
    strcpy(res, "1");
    char *temp;
    while (i < n)
    {
        temp = num;
        num = intal_add(num, "1");
        free(temp);

        temp = res;
        res = intal_multiply(res, num);
        free(temp);

        ++i;
    }
    free(num);

    return res;
}

char *intal_bincoeff(unsigned int n, unsigned int k)
{
    if(k > n/2)
    {
        k = n - k;
    }
    char **dp = (char **)malloc(sizeof(char *) * (k + 1));
    for (int i = 0; i < k + 1; ++i)
    {
        dp[i] = (char *)malloc(sizeof(char) * 2);
        strcpy(dp[i], "0");
    }
    strcpy(dp[0], "1");
    if (n == 0)
    {
        printf("here1");
        char *res;
        res = (char *)malloc(sizeof(char) * 2);
        strcpy(res, dp[k]);
        free(dp);
        return res;
    }
    char *temp;
    //printf("here");
    for (int n_i = 1; n_i <= n; ++n_i)
    {
        for (int k_i = k; k_i > 0; --k_i)
        {
            //printf("%s ", dp[k_i]);
            temp = dp[k_i];
            dp[k_i] = intal_add(dp[k_i], dp[k_i - 1]);
            free(temp);
        }
    }
    char *res = (char *)malloc(sizeof(char) * (strlen(dp[k]) + 1));
    strcpy(res, dp[k]);
    for (int i = 0; i < k + 1; ++i)
    {
        free(dp[i]);
    }
    free(dp);
    return res;
}

int intal_max(char **arr, int n)
{
    int index = 0;
    for (int i = 1; i < n; ++i)
    {
        if (intal_compare(arr[i], arr[index]) == 1)
        {
            index = i;
        }
    }
    return index;
}

int intal_min(char **arr, int n)
{
    int index = 0;
    for (int i = 1; i < n; ++i)
    {
        if (intal_compare(arr[i], arr[index]) == -1)
        {
            index = i;
        }
    }
    return index;
}

int intal_search(char **arr, int n, const char *key)
{
    for (int i = 1; i < n; ++i)
    {
        if (intal_compare(arr[i], key) == 0)
        {
            return i;
        }
    }
    return -1;
}

static void merge(char **arr, int low, int mid, int high)
{
    int n1 = mid - low + 1;
    int n2 = high - mid;
    //create two temporary sub arrays
    char **left = (char **)malloc(sizeof(char *) * n1);
    char **right = (char **)malloc(sizeof(char *) * n2);
    //copy the two parts into the temporary arrays
    //(we do not copy entire intals, only copy the address)
    for (int i = 0; i < n1; ++i)
    {
        left[i] = arr[low + i];
    }
    for (int j = 0; j < n2; ++j)
    {
        right[j] = arr[mid + 1 + j];
    }
    //merge the temp arrays back into original array
    int i = 0;
    int j = 0;
    int k = low;
    while (i < n1 && j < n2)
    {
        if (intal_compare(left[i], right[j]) <= 0)
        {
            arr[k] = left[i];
            ++i;
        }
        else
        {
            arr[k] = right[j];
            ++j;
        }
        ++k;
    }
    //There might be elements left over in one of the two sub arrays
    //copy remaining elements from left sub array, if any:
    while (i < n1)
    {
        arr[k] = left[i];
        ++k;
        ++i;
    }
    //copy remaining elements from left sub array, if any:
    while (j < n2)
    {
        arr[k] = right[j];
        ++k;
        ++j;
    }
    free(left);
    free(right);
}

static void mergeSort(char **arr, int low, int high)
{
    //printf("here");
    if (low < high)
    {
        int mid = (low + high) / 2;
        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);

        merge(arr, low, mid, high);
    }
}

void intal_sort(char **arr, int n)
{
    mergeSort(arr, 0, n - 1);
}

int intal_binsearch(char **arr, int n, const char *key)
{
    int low, mid, high;
    //mid = n/2;
    low = 0;
    high = n - 1;
    while (low <= high)
    {
        mid = (high + low) / 2;
        int compare = intal_compare(arr[mid], key);
        if (compare == 0)
        {
            int i = mid;
            while(i > 0 && intal_compare(arr[i - 1], key) == 0)
            {
                --i;
            }
            return i;
        }
        if (compare == -1)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return -1;
}

char *coin_row_problem(char **arr, int n)
{
    char* a0 = (char *)malloc(sizeof(char) * 2);
    char* a1 = (char *)malloc(sizeof(char) * (strlen(arr[0]) + 1));
    strcpy(a0, "0");
    strcpy(a1, arr[0]);
    if(n == 0)
    {
        free(a1);
        return a0;
    }
    if(n == 1)
    {
        free(a0);
        return a1;
    }
    char* a2 = (char*)malloc(sizeof(char) * 2);//allocating dynamic memory so as to use realloc function later
    strcpy(a2, "0");//a random intal
    for (int i = 2; i < n + 1; ++i)
    {
        char *sum = intal_add(arr[i - 1], a0);
        int compare = intal_compare(sum, a1);
        if(compare > 0)
        {
            a2 = realloc(a2, sizeof(char) * (strlen(sum) + 1));
            strcpy(a2, sum);
            free(sum);
        }
        else
        {
            free(sum);
            a2 = realloc(a2, sizeof(char) * (strlen(a1) + 1));
            strcpy(a2, a1);
        }
        

        a0 = realloc(a0, sizeof(char) * (strlen(a1) + 1));
        strcpy(a0, a1);

        a1 = realloc(a1, sizeof(char) * (strlen(a2) + 1));
        strcpy(a1, a2);
    }
    free(a0);
    free(a1);
    return a2;
}