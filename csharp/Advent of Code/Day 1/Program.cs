using System.Text.RegularExpressions;

class Day1
{
    static int ToInteger(string input)
    {
        switch (input)
        {
            case "zero": return 0;
            case "one": return 1;
            case "two": return 2;
            case "three": return 3;
            case "four": return 4;
            case "five": return 5;
            case "six": return 6;
            case "seven": return 7;
            case "eight": return 8;
            case "nine": return 9;
            default:
                return Int32.Parse(input);
        }
    }

    static void Main(string[] args)
    {
        foreach (var arg in args)
        {
            try
            {
                string line;
                var digit = "[0-9]";
                var digitsAndWords = "[0-9]|zero|one|two|three|four|five|six|seven|eight|nine";
                var simpleSum = 0;
                var complexSum = 0;

                using var reader = new StreamReader(arg);

                string firstDigit = "", lastDigit = "";
                string firstNumber = "", lastNumber = "";

                while ((line = reader.ReadLine()) != null)
                {
                    var firstMatch = Regex.Match(line, digit);
                    if (firstMatch.Success)
                    {
                        firstDigit = firstMatch.Captures[0].Value;
                        lastDigit = firstMatch.Captures[0].Value;
                    }

                    var firstNumberMatch = Regex.Match(line, digitsAndWords);
                    if (firstNumberMatch.Success)
                    {
                        firstNumber = firstNumberMatch.Captures[0].Value;
                        lastNumber = firstNumberMatch.Captures[0].Value;
                    }

                    var lastMatch = Regex.Match(line, digit, RegexOptions.RightToLeft);

                    if (lastMatch.Success)
                    {
                        lastDigit = lastMatch.Captures[0].Value;
                    }

                    var lastNumberMatch = Regex.Match(line, digitsAndWords, RegexOptions.RightToLeft);
                    if (lastNumberMatch.Success)
                    {
                        lastNumber = lastNumberMatch.Captures[0].Value;
                    }
                        
                    if (firstDigit.Length > 0 && lastDigit.Length > 0)
                    {
                        var combined = (firstDigit + lastDigit).Trim();
                        var number = Int32.Parse(combined);
                        simpleSum += number;
                    }

                    if (firstNumber.Length > 0 && lastNumber.Length > 0)
                    {
                        complexSum += (ToInteger(firstNumber) * 10) + (ToInteger(lastNumber));
                    }
                }

                Console.WriteLine("Part 1 Sum: {0}", simpleSum);
                Console.WriteLine("Part 2 Sum: {0}", complexSum);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.ToString());
            }
        }
    }
}