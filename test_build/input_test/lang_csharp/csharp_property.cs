/// <summary>
/// Test properties in C#
/// </summary>
public class CSharp_Property
{
	/// <summary>
	/// First test: static property, default 55
	/// </summary>
	public int a { get; set; } = 55;
	
	/// <summary>
	/// Second test: non-static property, default 5.0
	/// </summary>
	public double b { get; set; } = 5.0;
	
	/// <summary>
	/// Third test: char, default 'c'
	/// </summary>
	public char c {get; set;} = 'c';
	
	/// <summary>
	/// Fourth test: byte, no default
	/// <summary>
	public byte d {get;set;}
	
	/// <summary>
	/// Main function
	/// </summary>
	/// <param name="args">Command-line arguments</param>
	public void Main(string[] args)
	{
		
	}
}